#include "MultiplyToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

MultiplyToken::MultiplyToken() {

}
MultiplyToken::~MultiplyToken() {

}
EScriptToken MultiplyToken::GetType() {
    return ESCRIPTTOKEN_MULTIPLY;
}
void MultiplyToken::LoadParams(IQStream *stream) {

}
std::string MultiplyToken::ToString() {
    return " * ";
}