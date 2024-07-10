#include "ScriptToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ScriptToken::ScriptToken() {

}
ScriptToken::~ScriptToken() {

}
EScriptToken ScriptToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_SCRIPT;
}
void ScriptToken::LoadParams(IQStream *stream) {

}
std::string ScriptToken::ToString() {
    return "script ";
}
int	ScriptToken::GetPostTabOffset() {
    return 1;
}