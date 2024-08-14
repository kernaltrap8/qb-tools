#include "StructureSymbol.h"
#include "ReferenceItemSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

StructureSymbol::StructureSymbol() {

}
StructureSymbol::~StructureSymbol() {

}
ESymbolType StructureSymbol::GetType() {
    return ESYMBOLTYPE_STRUCTURE;
}
QSymbolToken *StructureSymbol::NextSymbol(IStream *stream) {
    uint16_t flags = stream->ReadUInt16();
    uint8_t type = stream->ReadByte();
    stream->ReadByte();
    
    QSymbolToken *token = QSymbolToken::Resolve(type);
    token->LoadParams(stream);
    return token;
}
void StructureSymbol::LoadParamsFromArray(IStream *stream) {
    uint32_t hdr = stream->ReadUInt32();
    assert(hdr == 256);

    uint32_t offset = stream->ReadUInt32();
    stream->SetCursor(offset);
    while(true) {
        uint8_t unk = stream->ReadByte();
        uint8_t type_flags = stream->ReadByte();
        assert(type_flags & 0x80);
        uint8_t type = type_flags & 0xF;

        uint16_t unk2 = stream->ReadUInt16();
        assert(unk2 == 0);

        uint32_t name = stream->ReadUInt32();

        QSymbolToken *token = NULL;
        if(type_flags & 0x10) { //is reference
            token = new ReferenceItemSymbol(type_flags & 0xF);
        } else {
            token = QSymbolToken::Resolve(type);
        }
        token->SetIsStructItem(true);
        token->LoadParams(stream);
        token->SetNameChecksum(name);
        m_children.push_back(token);

        uint32_t next = token->GetNextOffset();
        if(next == 0) {
            break;
        }
        stream->SetCursor(next);
    }
}
void StructureSymbol::LoadParams(IStream *stream) {
    uint32_t struct_offset = stream->ReadUInt32();
    m_next_offset = stream->ReadUInt32();

    stream->SetCursor(struct_offset);

    LoadParamsNoOffset(stream);
}
void StructureSymbol::LoadParamsNoOffset(IStream *stream) {
    uint32_t hdr = stream->ReadUInt32();
    assert(hdr == 256);

    uint32_t offset = stream->ReadUInt32();
    assert(offset == 0 || offset == stream->GetOffset());
    
    if(offset == 0) { //if 0, emptry struct
        return;
    }
    while(true) {
        uint8_t unk = stream->ReadByte();
        uint8_t type_flags = stream->ReadByte();
        assert(type_flags & 0x80);
        uint8_t type = type_flags & 0xF;

        uint16_t unk2 = stream->ReadUInt16();
        assert(unk2 == 0);

        uint32_t name = stream->ReadUInt32();

        QSymbolToken *token = NULL;
        if(type_flags & 0x10) { //is reference
            ReferenceItemSymbol *ref = new ReferenceItemSymbol(type_flags & 0xF);
            ref->SetIsStructItem(true);
            ref->SetValue(stream->ReadUInt32());
            ref->SetNextOffset(stream->ReadUInt32());
            token = ref;
        } else {
            token = QSymbolToken::Resolve(type);
            token->SetIsStructItem(true);
            token->LoadParams(stream);
        }
        token->SetNameChecksum(name);
        m_children.push_back(token);


        uint32_t next = token->GetNextOffset();
        if(next == 0) {
            break;
        }

        stream->SetCursor(next);
    }
}
#include <cassert>
void StructureSymbol::Write(IStream *stream) {
    assert(0);
}
void StructureSymbol::WriteToArray(IStream *stream) {
    assert(0);
}
std::string StructureSymbol::ToString() {
    std::ostringstream ss;
    
    return ss.str();
}