#include "SwitchToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

SwitchToken::SwitchToken() {

}
SwitchToken::~SwitchToken() {

}
EScriptToken SwitchToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_SWITCH;
}
void SwitchToken::LoadParams(IQStream *stream) {

}
std::string SwitchToken::ToString() {
    return "switch ";
}