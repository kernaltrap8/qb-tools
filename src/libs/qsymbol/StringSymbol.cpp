#include "StringSymbol.h"

#include <iostream>
#include <sstream>
#include <iomanip>

StringSymbol::StringSymbol() {

}
StringSymbol::~StringSymbol() {

}
ESymbolType StringSymbol::GetType() {
    return ESYMBOLTYPE_STRING;
}
void StringSymbol::LoadParams(IStream *stream) {
    uint32_t offset = stream->ReadUInt32();    
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    }
    stream->SetCursor(offset);

    const int MAX_CHARS = 256;
    char data[MAX_CHARS];
    uint32_t i = 0;

    while(true) {
        data[i % MAX_CHARS] = (char)stream->ReadByte();
        if(data[i % MAX_CHARS] == 0) {
            break;
        }
        i++;
    }

    m_value = (char *)&data[0];
    
    stream->Align();
}
void StringSymbol::LoadParamsFromArray(IStream *stream) {
    const int MAX_CHARS = 256;
    char data[MAX_CHARS];
    uint32_t i = 0;

    while(true) {
        data[i % MAX_CHARS] = (char)stream->ReadByte();
        if(data[i % MAX_CHARS] == 0) {
            break;
        }
        i++;
    }

    m_value = (char *)&data[0];
    stream->Align();
}
#include <cassert>
void StringSymbol::Write(IStream *stream) {
    assert(0);
}
void StringSymbol::WriteToArray(IStream *stream) {
    assert(0);
}
std::string StringSymbol::ToString() {
    return "";
}