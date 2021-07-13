#include <iostream>
#include <bits/stdc++.h>
using namespace std;
vector <string> XML_string;
vector<string> XML_string_without_slashes;
vector<string> lines;
vector<string> tags;
string json="{";
struct Node{
    string data;
    vector<Node *> children;
    Node* parent;
    string inner_data;
};

Node* new_node(string data){
    if(data.find('=')==-1){
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
    return temp;
}
void add_child(Node* root,Node* child){
    root->children.push_back(child);
    return;
}
Node* get_parent(Node* root){
    Node* temp=root;
    if(temp->parent!=nullptr)
        root=temp->parent;
    return root;
}
Node* get_root(Node* root){
    Node* temp=root;
    while(true){
        if(temp->parent==nullptr){
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
    if(root->children.size()<1)
        return;
    vector<string> temp;
    vector<string> tag_names;
    for(unsigned int i=0;i<root->children.size();i++){
        if(root->data!="*")
            temp.push_back(root->children[i]->data);
    }
    for(unsigned int i=0;i<temp.size();i++){
        if(count(temp.begin(),temp.end(),temp[i])>1&&count(tag_names.begin(),tag_names.end(),temp[i])==0){
            tag_names.push_back(tag_names[i]);
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
get_xml_without_slahes(){
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
