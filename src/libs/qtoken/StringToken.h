#ifndef _STRINGTOKEN_H
#define _STRINGTOKEN_H
#include "QScriptToken.h"

class StringToken : public QScriptToken {
    public:
        StringToken();
        ~StringToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
        std::string m_value;
};
#endif //_STRINGTOKEN_H