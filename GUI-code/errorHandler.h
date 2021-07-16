#ifndef  _ERRORHANDLER_
#define  _ERRORHANDLER_

//Line position calculations

#define    STARTERR_TE      (otag_match.position(1) + old_pos)
#define    ENDERR_TE        (STARTERR_TE + otag_match.str(1).size() - 1)
#define    STARTERR_EUL     (otag_match.position(1) - (otag_match.prefix().str().size()) + old_pos)
#define    ENDERR_EUL       (otag_match.position(1) + old_pos - 1)
#define    STARTERR_TBIT    (ctag_match.position(1) - text.size() + old_pos)
#define    ENDERR_TBIT      (ctag_match.position(1) + old_pos - 1)
#define    STARTERR_TOOO    (ctag_match.position(1) + old_pos)
#define    ENDERR_TOOO      (STARTERR_TOOO + ctag_match.str(1).size() - 1)

//Data sharing with the GUI

struct GUImessege   {

    int errStart;
    int errEnd;
    std::string error{},errorDescription{},errorFix{};

};

//Exception Handler

class Error : public std::exception {
    public:
    typedef enum {extraUselessText, tagBracketsInText, tagsOutOfOrder, tagError, closingTagError} err;
    err e;
    int start{0},end{0};
    std::string errorLine{""};

    Error(err er,int st,int en,std::string errL) : e(er),start(st),end(en),errorLine(errL)   {

    }

    void sendMessege(GUImessege);
    void recieveMessege();
    void fixEUL();
    std::string fixTBIT();
    void fixTOOO();
    std::string fixTE();
    std::string fixCTE();
};

#endif //_ERRORHANDLER_