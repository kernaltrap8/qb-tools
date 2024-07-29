#include "ArraySymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

ArraySymbol::ArraySymbol() {

}
ArraySymbol::~ArraySymbol() {

}
ESymbolType ArraySymbol::GetType() {
    return ESYMBOLTYPE_ARRAY;
}
QSymbolToken *NextSymbol(IStream *stream, uint8_t type) {
    QSymbolToken *token = QSymbolToken::Resolve(type);
    token->LoadParamsFromArray(stream);
    return token;
}
void ArraySymbol::LoadParams(IStream *stream) {
    uint32_t array_data_offset = stream->ReadUInt32();

    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    }
    stream->SetCursor(array_data_offset);

    uint16_t hdr = stream->ReadUInt16();
    assert(hdr == 1);

    uint8_t type = stream->ReadByte();
    stream->ReadByte();

    m_num_items = stream->ReadUInt32();

    m_tokens = new QSymbolToken*[m_num_items];

    ReadSymbolsFromArray(stream, type, m_num_items, m_tokens);
}

void ArraySymbol::LoadParamsFromArray(IStream *stream) {
    printf("IMPLEMENT ArraySymbol::LoadParamsFromArray\n");
    assert(false);
}
std::string ArraySymbol::ToString() {
    return "";
}