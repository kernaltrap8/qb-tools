#ifndef _LOCALSTRINGSYMBOL_H
#define _LOCALSTRINGSYMBOL_H
#include "QSymbolToken.h"

class LocalStringSymbol : public QSymbolToken {
    public:
        LocalStringSymbol();
        ~LocalStringSymbol();
        ESymbolType GetType();
        std::string GetValue() { return m_value; }
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        std::string m_value;
};
#endif //_LOCALSTRINGSYMBOL_H