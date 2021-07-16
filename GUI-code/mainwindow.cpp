#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QFile out_file("out.txt");
QFile in_file("in.txt");
vector <string> lines;
QString text;
void readFile(){

    in_file.open(QIODevice::ReadOnly |QIODevice::Text);
    std::string line;
    lines.resize(0);
    while (!in_file.atEnd())
    { line = (in_file.readLine().trimmed()).toStdString();
      lines.push_back(line);
    }

    in_file.close();

}

vector <string> XML_string;
vector<string> XML_string_without_slashes;
vector<string> tags;
string json="{";
struct Node{
    string data;
    vector<Node *> children;
    Node* parent;
    string inner_data;
};

Node* new_node(string data){
    if(data.find('=')==string::npos){
        Node* temp=new Node;
        temp->data=data;
        return temp;
    }
    else{
        int index=data.find(' ');
        string tag=data.substr(0,index);
        Node* temp=new Node;
        temp->data=tag;
        temp->inner_data=data.substr(index+1,data.length()-1);
        return temp;
    }
}
Node* add_child(Node* root,string data){
    Node* temp=new_node(data);
    root->children.push_back(temp);
    temp->parent=root;
    return temp;
}
void add_child(Node* root,Node* child){
    root->children.push_back(child);
    return;
}
Node* get_parent(Node* root){
    Node* temp = root;
    if(temp->parent != NULL){
       root = temp->parent;
    }
    return root;
}
Node* get_root(Node* root){
    Node* temp=root;
    while(true){
        if(temp->parent==NULL){
            break;
        }
        else{
            temp=temp->parent;
        }
    }
    return temp;
}
Node* str_to_tree(Node* curr_root){
    stack<string> st;
    for(unsigned int i=0;i<XML_string_without_slashes.size();i++){
        if(i==0){
            curr_root=new_node(XML_string_without_slashes[i]);
            curr_root->parent=nullptr;
            stringstream s1(XML_string_without_slashes[i]);
            string s;
            getline(s1,s,' ');
            st.push(s);
            continue;
        }
        if(XML_string_without_slashes[i][0]!='+'){
            stringstream s1(XML_string_without_slashes[i]);
            string s;
            getline(s1,s,' ');
            if(s==st.top()){
                st.pop();
                curr_root=get_parent(curr_root);
            }
            else{
                curr_root=add_child(curr_root,XML_string_without_slashes[i]);
                st.push(s);
            }
        }
        else{
            curr_root=add_child(curr_root,XML_string_without_slashes[i].substr(1,XML_string_without_slashes[i].length()-1));
            curr_root=get_parent(curr_root);
        }
    }
    return get_root(curr_root);
}
void delete_repeated_children(Node* root){
    if(root->children.size()==0)
        return;
    vector<string> temp;
    vector<string> tag_names;
    for(unsigned int i=0;i<root->children.size();i++){
        if(root->children[i]->data!="*")
            temp.push_back(root->children[i]->data);
    }
    for(unsigned int i=0;i<temp.size();i++){
        if(count(temp.begin(),temp.end(),temp[i])>1&&count(tag_names.begin(),tag_names.end(),temp[i])==0){
            tag_names.push_back(temp[i]);
        }
    }
    for(unsigned int i=0;i<tag_names.size();i++){
        Node* sub=new_node(tag_names[i]);
        for(unsigned int j=0;j<root->children.size();j++){
            if(tag_names[i]==root->children[j]->data){
                root->children[j]->data='*';
                root->children[j]->parent=sub;
                add_child(sub,root->children[j]);
                root->children.erase(root->children.begin()+j);
                j--;
            }
        }
        add_child(root,sub);
        sub->parent=root;
    }
    return;
}
void organize_tree(Node* root){
    if(root==nullptr)
        return;
    delete_repeated_children(root);
    for(unsigned int i=0;i<root->children.size();i++){
        organize_tree(root->children[i]);
    }
    return;
}

void add_qutations(Node* root){
    for(unsigned int i =0;i<root->children.size();i++){
        if(root->children[i]->data!="*")
            root->children[i]->data="\""+root->children[i]->data+"\"";
        add_qutations(root->children[i]);
    }
}
Node* get_last_child(Node* root){
    if(root->children.size()==0)
        return root;
    return get_last_child(root->children[root->children.size()-1]);
}
void add_brackets(Node* root){
    for(unsigned int i=0;i<root->children.size();i++){
        if((root->children.size() >= 1) && (root->children[0]->data != "*") && (root->children[i]->children.size() != 0) && (i == root->children.size()-1)){
            Node* temp=get_last_child(root);
            if(temp->data[temp->data.length()-1]==']'||(temp->data[temp->data.length()-1] == '}')){
                int sq_count=count(temp->data.begin(),temp->data.end(),']');
                int curly_count=count(temp->data.begin(),temp->data.end(),'}');
                if(sq_count<0)
                    sq_count=0;
                if(curly_count<0)
                    curly_count=0;
                int sum=sq_count+curly_count;
                temp->data.insert(temp->data.length()-sum,"}");
            }
            else{
                temp->data = temp->data + "}";
            }

        }
        else if((root->children.size() > 1) && (root->children[0]->data == "*") && (i == root->children.size()-1)){
            Node* temp=get_last_child(root->children[i]);
            if(temp->data[temp->data.length()-1]==']'||(temp->data[temp->data.length()-1] == '}')){
                int sq_count=count(temp->data.begin(),temp->data.end(),']');
                int curly_count=count(temp->data.begin(),temp->data.end(),'}');
                if(sq_count<0)
                    sq_count=0;
                if(curly_count<0)
                    curly_count=0;
                int sum=sq_count+curly_count;
                temp->data.insert(temp->data.length()-sum,"]");
            }
            else{
                temp->data = temp->data + "]";
            }
        }
        add_brackets(root->children[i]);
    }
}
void handle_inner_data(Node* root){
    if(!(root->inner_data.empty())){
        for(unsigned int i=0;i<root->inner_data.length();i++){
            if(i==0){
                root->inner_data="{\"@"+root->inner_data;
            }
            if(root->inner_data[i]==' '&&root->inner_data[i-1]=='"'){
                root->inner_data[i]=',';
                root->inner_data.insert(i+1,"\"");
                root->inner_data.insert(i+2,"@");
            }
            else if(root->inner_data[i]=='='){
                root->inner_data[i]=':';
                root->inner_data.insert(i,"\"");
                i++;
                }
            }
        root->inner_data+=",";
        }
    }

void handle_inner_data_tree(Node* root){
    handle_inner_data(root);
    for(unsigned int i=0;i<root->children.size();i++){
        handle_inner_data_tree(root->children[i]);
    }
}
void node_to_json(Node* root){
    if(root->data=="\*"){
        json+= root->inner_data;
    }
    else if(root->children.size()==0){
        if(root->parent->inner_data.empty()&&(root->data[root->data.length()-1]!='}'||root->data[root->data.length()-1]!=']'))
            json+=root->data+",";
        else if(root->parent->inner_data.empty()){
            json+=root->data;
        }
        else if(!root->parent->inner_data.empty()&&(root->parent->children.size()==1)&&(root->data[root->data.length()-1] != '}' || root->data[root->data.length()-1] != ']')){
            json+="\"#text\":"+root->data+"},";
        }
    }
    else if(root->children.size()>0&&root->children[0]->data=="\*"){
        json+=root->data+":[";
    }
    else if(root->children.size()>=1){
        if(root->inner_data.empty()){
            json+=root->data+":";
        }
        else if(!root->inner_data.empty()){
            json+=root->data+":"+root->inner_data;
        }
    }
}
void tree_to_json(Node* root){
    node_to_json(root);
    for(unsigned int i=0;i<root->children.size();i++)
        tree_to_json(root->children[i]);
    return;
}
void to_json_string(Node* root){
    root->data="\""+root->data+"\"";
    add_qutations(root);
    add_brackets(root);
    tree_to_json(root);
    json+="}";
}
void get_xml_string(){
vector<string> tags_with_empty_lines;
tags.resize(0);
XML_string.resize(0);
for(unsigned long long i =0;i<lines.size();i++){
    int counter=count(lines[i].begin(),lines[i].end(),'<');
    int start=lines[i].find('<');
    int last=lines[i].find('>');
    for(int j=0;j<counter;j++){
        tags.push_back(lines[i].substr(start+1,last-start-1));
        tags_with_empty_lines.push_back(lines[i].substr(start+1,last-start-1));
        if(lines[i][last+1]!='<'){
            int temp=lines[i].find('<',start+1);
            tags_with_empty_lines.push_back('+'+lines[i].substr(last+1,temp-last-1));
        }
        int start2=lines[i].find('<',start+1);
        int last2=lines[i].find('>',last+1);
        last=last2;
        start=start2;
    }

}
for(unsigned long long j=0;j<tags_with_empty_lines.size();j++){
    if(!(tags_with_empty_lines[j][0]=='+'&&tags_with_empty_lines[j].length()==1)){

        XML_string.push_back(tags_with_empty_lines[j]);
    }
}
}
void get_xml_without_slahes(){
XML_string_without_slashes.resize(0);
    for(unsigned int i=0;i<XML_string.size();i++){
        if(XML_string[i][0]=='/'){
            XML_string_without_slashes.push_back(XML_string[i].substr(1,XML_string[i].length()-1));
        }
        else{
            XML_string_without_slashes.push_back(XML_string[i]);
        }
    }
}
void make_all(Node* root){
readFile();

get_xml_string();

get_xml_without_slahes();

root=NULL;

root = str_to_tree(root);      //make Tree, return main Node

organize_tree(root);                                           //organize tree, as if repeated node get merged

handle_inner_data_tree(root);

to_json_string(root);
}
void MainWindow::on_OpenFile_clicked()
{
    ui->input_text->clear();
    QFile input_file(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("XML File (*.xml) ;;TextFile (*.txt)")));
    input_file.open(QIODevice::ReadOnly |QIODevice::Text);
    QTextStream stream(&input_file);
    text= stream.readAll();
    in_file.remove();
    out_file.resize(0);
    input_file.copy("in.txt");
    QFile in_file("in.txt");
    ui->input_text->setPlainText(text);
    ui->input_text->setLineWrapMode(QPlainTextEdit::NoWrap);
   input_file.close();
}
void MainWindow::on_TOJson_clicked()
{
    out_file.resize(0);
    out_file.open(QIODevice::ReadWrite |QIODevice::Text);
    QTextStream st(& out_file);

     Node* root=new Node;
     make_all(root);
     st<<QString::fromStdString(json);         //print json
     out_file.close();
     out_file.open(QIODevice::ReadWrite |QIODevice::Text);
     QTextStream strq(&out_file);
     ui->output_text->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);
     ui->output_text->clear();
     QString out=strq.readAll();
     QChar prevchar;
      int level=0;
      for(auto i:out)
     {
         if(i=='{'){
             ui->output_text->insertPlainText("\n");
             for(int j=0;j<level;j++){
                 ui->output_text->insertPlainText("   ");
             }
             ui->output_text->insertPlainText(i);
             ui->output_text->insertPlainText("\n");
             level=level+1;
             for(int j=0;j<level;j++){
                 ui->output_text->insertPlainText("   ");
             }
         }
         else if(i=='}'){

             ui->output_text->insertPlainText("\n");
             level=level-1;
             for(int j=0;j<level;j++){
                 ui->output_text->insertPlainText("   ");
             }
             ui->output_text->insertPlainText(i);
             }
         else if(i=='['){
             ui->output_text->insertPlainText("\n");
             for(int j=0;j<level;j++){
                 ui->output_text->insertPlainText("   ");
             }
             ui->output_text->insertPlainText(i);
             ui->output_text->insertPlainText("\n");
             level=level+1;
             for(int j=0;j<level;j++){
                 ui->output_text->insertPlainText("   ");
             }
         }
         else if(i==']'){

             ui->output_text->insertPlainText("\n");
             level=level-1;
             for(int j=0;j<level;j++){
                 ui->output_text->insertPlainText("   ");
             }
              ui->output_text->insertPlainText(i);
              }
         else if(i==',')
         {
             ui->output_text->insertPlainText(i);
             ui->output_text->insertPlainText("\n");
             for(int j=0;j<level;j++){
                 ui->output_text->insertPlainText("   ");
             }
         }
         else {ui->output_text->insertPlainText(i);}
         prevchar=i;
     }
     out_file.close();
     json = "{";
     lines.resize(0);
     tags.resize(0);
     XML_string.resize(0);
     XML_string_without_slashes.resize(0);
     return;
}

void MainWindow::on_Save_clicked()
{
    QFile output_file(QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Text File ()*.txt;;XML File ()*.xml")));
    output_file.open(QIODevice::ReadWrite|QIODevice::Text);
    QString text=ui->output_text->toPlainText();
    output_file.write(text.toUtf8());
    output_file.close();
}

vector<int> check_and_correct(vector<string> & in, bool correct)
{
    vector<int> error_lines;
    stack <pair<int, string>> tags; //stack has open tag and which line the tag is in
    int l = 0;
    int m = in.size();
    for (int k = 0; k < m; k++)
    {
        int n = in[k].length();
        for (int i = 0; i < n; i++)
        {
            string tag, o_tag, c_tag;
            if (in[k][i] == '<' && in[k][i + 1] != '?' && in[k][i + 1] != '!') //found a tag
            {
                i++;
                while (i < in[k].length() && in[k][i] != '>')
                {
                    tag += in[k][i++];
                }

                if (isalpha(tag[0]) && tag[tag.length() - 1] != '/') // found open tag
                {
                    int j = 0;
                    while (j < tag.length() && tag[j] != ' ')
                    {
                        o_tag += tag[j++];
                    }
                    auto t = make_pair(l, o_tag);
                    tags.push(t);
                }
                else if (tag[0] == '/') //found closing tag
                {
                    c_tag = tag.substr(1);
                    if (c_tag == tags.top().second) tags.pop();
                    else if (correct) //add closing tag to end of line in open tag and check stack top again
                    {
                        error_lines.push_back(tags.top().first+1);
                        while (c_tag != tags.top().second) {
                            int line = tags.top().first;
                            string t = "</" + tags.top().second + '>';
                            in[line] = in[line].substr(0, in[line].size() - 1) + t + '\n';
                            tags.pop();
                        }
                    }
                    else {
                        error_lines.push_back(tags.top().first); break;
                    }
                }
            }
        }
        l++;
        in[k] += '\n';
    }
    sort(error_lines.begin(),error_lines.end());
    return error_lines;
}
vector<int> mistake_case;
vector<int> mistakes;
vector<string> tags_new;
void find_mistakes(){
    for(unsigned int i=0;i<lines.size();i++){
        if(lines[i].empty())
        {
            tags_new.push_back(lines[i]);
            continue;
        }
        if(lines[i][1]=='?'||lines[i][1]=='!'||lines[i][lines[i].length()-2]=='/')
            continue;
        int counter=count(lines[i].begin(),lines[i].end(),'<');
        int start=lines[i].find('<');
        int last=lines[i].find('>');
        if(counter==0)
        {
            tags_new.push_back("+"+lines[i]);
            continue;
        }
        for(int j=0;j<counter;j++){
            if(j==0){
                string s=lines[i].substr(start+1,last-start-1);
                s=s.substr(0,s.find(' '));
                tags_new.push_back(s);
            }
            else{
                string s=lines[i].substr(start+1,last-start-1);
                s=s.substr(0,s.find(' '));
                tags_new.back()=tags_new.back()+"-"+s;
            }
           int start2=lines[i].find('<',start+1);
           int last2=lines[i].find('>',last+1);
           start=start2;
           last=last2;
        }
    }
    vector <string> helper;
    vector<int> index;
    for(unsigned int x=1;x<tags_new.size()+1;x++){

        if(tags_new[x-1].empty()){
            mistakes.push_back(x);
            continue;
        }else if(tags_new[x-1][0] == '+'){
            continue;
        }

        if(tags_new[x-1].find('/') == std::string::npos){
            helper.push_back(tags_new[x-1]);
            index.push_back(x);
        }else{
        std::stringstream check1(tags_new[x-1]);

        std::string intermediate;

        while(getline(check1, intermediate, '-'))
        {
            if(intermediate.find('/') == std::string::npos){
                helper.push_back(intermediate);
                index.push_back(x);
            }else{
                std::string s = intermediate.substr(1,intermediate.length()-1);
                if( helper.back() == s ){
                    helper.pop_back();
                    index.pop_back();
                }else if( helper[helper.size()-2] == s ){
                    mistakes.push_back(index.back());
                    mistake_case.push_back(1);
                    helper.pop_back();
                    index.pop_back();
                    x--;
                }else{
                    mistakes.push_back(x);
                    mistake_case.push_back(2);
                    helper.pop_back();
                    index.pop_back();
                }
            }
        }
       }
      }


      sort(mistakes.begin(), mistakes.end());
    return;
}

void correctMistakes(){
  if(mistake_case.size() > 0){
   for(unsigned int x=0;x<mistakes.size();x++){

    if(mistake_case[x] == 2){
        std::string s;
        std::stringstream check1( lines[mistakes[x]-1] );
        getline(check1, lines[mistakes[x]-1] , '/');
        std::string temp = lines[mistakes[x]-1].substr(0,lines[mistakes[x]-1].length()-1);
        std::stringstream check2( temp );
        getline(check2, s , '>');
        lines[mistakes[x]-1] = temp + "</" + s.substr(1,s.find_first_of(' ')) +">";

    }if(mistake_case[x] == 1 && lines[mistakes[x]-1][lines[mistakes[x]-1].length()-1] != '>'){
        std::string s;
        std::stringstream check1( lines[mistakes[x]-1] );
        getline(check1, s , '>');
        lines[mistakes[x]-1] = lines[mistakes[x]-1] + "</" + s.substr(1,s.find_first_of(' ')) +">";
    }else if(mistake_case[x] == 1){
        std::string s;
        std::stringstream check1( lines[mistakes[x]-1] );
        getline(check1, s , '>');
        for(unsigned int y=0;y<lines.size();y++){
            if(lines[y].empty()){
                lines[y] = "</" + s.substr(1,s.find_first_of(' ')) +">";
            }
        }
    }

   }
  }
  return;
}
//returns single string without whitespaces and newlines
string minify(vector<string> in)
{
    string out;
    string s;
    for(int i=0;i<in.size();i++){
        if(in[i][0]=='+')
            out+=in[i].substr(1,in[i].length()-1);
        else
            out+="<"+in[i]+">";
    }
    return out;
}

//prettifies balanced xml file
string prettify(vector <string> in)
{
    int level=0;
    string out;
    string tab="     ";
    for(int i=0;i<in.size();i++){
        if(in[i][0]=='+'){
            for(int j=0;j<level;j++){
                out+=tab;}
            out+=in[i].substr(1,in[i].length()-1);
            out+="\n";

        }
        else if(in[i][0]=='/'){
            level--;
            for(int j=0;j<level;j++){
                out+=tab;}
            out+="<"+in[i]+">";
            out+="\n";

        }
        else if(in[i][in[i].length()-1]=='/'){
            for(int j=0;j<level;j++){
                out+=tab;}
            out+="<"+in[i]+">";
            out+="\n";
        }
        else{

            for(int j=0;j<level;j++){
                out+=tab;}
            out+="<"+in[i]+">";
            out+="\n";
            level++;
        }

    }
    return out;
}
void MainWindow::on_Check_clicked()
{

    ui->output_text->clear();

    std::string line;
    QTextCharFormat format;
    QTextCursor cursor( ui->output_text->textCursor() );
    readFile();
    vector<string> lines_copy(lines);
    vector<int> xx=check_and_correct(lines_copy,false);
    mistakes.resize(0);
   find_mistakes();
   if(xx.size() == 0)
    {
     QMessageBox enteredString;
     enteredString.setText("Correct XML File");
     enteredString.exec();
    }
   else
        {int j =0;
          for (unsigned int i=0;i<lines.size();i++)
          {line=lines[i];
              if(i == xx[j])
              {
                  format.setFontWeight( QFont::TypeWriter );
                  format.setForeground( QBrush( QColor(Qt::red) ) );
                  cursor.setCharFormat( format );
                  cursor.insertText(QString::fromStdString(line));
                  if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
                  j++;
              }
              else
              {
                  format.setFontWeight( QFont::TypeWriter );
                  format.setForeground( QBrush( QColor(Qt::black) ) );
                  cursor.setCharFormat( format );
                  cursor.insertText(QString::fromStdString(line));
                  if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
              }
          }
        }
}


void MainWindow::on_Correct_clicked()
{
    ui->output_text->clear();

    std::string line;
    QTextCharFormat format;
    QTextCursor cursor( ui->output_text->textCursor() );
    vector<string> lines_copy(lines);
    correctMistakes();
    int j =0;
              for (unsigned int i=1;i<lines.size()+1;i++)
              {line=lines[i-1];
                  if(i == mistakes[j])
                  {
                      format.setFontWeight( QFont::TypeWriter );
                      format.setForeground( QBrush( QColor(Qt::green) ) );
                      cursor.setCharFormat( format );
                      cursor.insertText(QString::fromStdString(line));
                      if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
                      j++;
                  }
                  else
                  {
                      format.setFontWeight( QFont::TypeWriter );
                      format.setForeground( QBrush( QColor(Qt::black) ) );
                      cursor.setCharFormat( format );
                      cursor.insertText(QString::fromStdString(line));
                      if(cursor.PreviousCharacter != '\n'){cursor.insertText("\n");}
                  }
              }
}





void MainWindow::on_Prettify_clicked()
{
    string out;
    readFile();
    get_xml_string();
    out=prettify(XML_string);
    QString output=QString::fromStdString(out);
    ui->output_text->setPlainText(output);
}


void MainWindow::on_Minify_clicked()
{
    string out;
    readFile();
    get_xml_string();
    out =minify(XML_string);
    ui->output_text->setPlainText(QString::fromStdString(out));

}


void MainWindow::on_Compress_clicked()
{
    Compressor co;
    string output=co.compress(text.toStdString());
    QFile output_file(QFileDialog::getSaveFileName(this,tr("Save File"),"",tr("Text File ()*.txt")));
    output_file.open(QIODevice::ReadWrite|QIODevice::Text);
    QString text=QString::fromStdString(output);
    output_file.write(text.toUtf8());
    output_file.close();
}


void MainWindow::on_decompress_clicked()
{
    ui->output_text->clear();
    QFile input_file(QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("XML File (*.xml) ;;TextFile (*.txt)")));
    input_file.open(QIODevice::ReadOnly |QIODevice::Text);
    QTextStream stream(&input_file);
    text= stream.readAll();
    string s=text.toStdString();
    Compressor co;
    string output=co.decompress(s);
    ui->output_text->setPlainText(QString::fromStdString(output));
}

