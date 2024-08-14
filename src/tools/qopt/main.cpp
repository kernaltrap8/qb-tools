#include <stdio.h>
#include <stdint.h>
#include "qopt.h"
#include <QStream.h>
#include <QScriptToken.h>
#include <NameToken.h>
#include <IntegerToken.h>
#include <FloatToken.h>

#include <SymbolFileStream.h>
#include <cassert>

Deopt g_Deopt;

void handle_read_token_value(QScriptToken *token) {
    g_Deopt.currentState = DeoptState_ReadNextGlobalToken;
    g_Deopt.currentToken = token;
    switch(token->GetType()) {
        case ESCRIPTTOKEN_INTEGER:
        case ESCRIPTTOKEN_FLOAT:
        case ESCRIPTTOKEN_NAME:
            emit_symbol();
            break;
        case ESCRIPTTOKEN_STARTARRAY:
            g_Deopt.currentState = DeoptState_ReadArrayTokens;
            g_Deopt.depth_index = 1;
            printf("root start array\n");
            break;
        case ESCRIPTTOKEN_STARTSTRUCT:
            g_Deopt.currentState = DeoptState_ReadStructTokens;
            g_Deopt.depth_index = 1;
            printf("root start struct\n");
            break;
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
        default:
            assert(false);
    }
}

void handle_global_token_state(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_NAME:
            g_Deopt.root_name_checksum = reinterpret_cast<NameToken*>(token)->GetChecksum();           
            g_Deopt.currentState = DeoptState_ReadEqualsToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_SCRIPT:
            g_Deopt.currentState = DeoptState_ReadScriptName;
            break;
        break;
        default:
            assert(false);
        case ESCRIPTTOKEN_CHECKSUM_NAME:
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
    }
}

void handle_read_script_name(QScriptToken *token) {
        switch(token->GetType()) {
            case ESCRIPTTOKEN_NAME:
                g_Deopt.root_name_checksum = reinterpret_cast<NameToken*>(token)->GetChecksum();
                g_Deopt.currentState = DeoptState_ReadScriptTokens;
            break;
            default:
                assert(false);
        }
}


void handle_equals_token_state(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_EQUALS:
            g_Deopt.currentState = DeoptState_ReadTokenValue;
        break;
        //ignore
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
        default:
            assert(false);
    }
}
void handle_read_root_array(QScriptToken *token) {
    if(token->GetType() == ESCRIPTTOKEN_ENDARRAY) {
        g_Deopt.depth_index--;
    } else if(token->GetType() == ESCRIPTTOKEN_STARTARRAY) {
        g_Deopt.depth_index++;
    }

    if(g_Deopt.depth_index == 0) {
        printf("Got end of array\n");
        g_Deopt.currentState = DeoptState_ReadNextGlobalToken;
    }
}
void handle_read_root_struct(QScriptToken *token) {
    if(token->GetType() == ESCRIPTTOKEN_ENDSTRUCT) {
        g_Deopt.depth_index--;
    } else if(token->GetType() == ESCRIPTTOKEN_STARTSTRUCT) {
        g_Deopt.depth_index++;
    }

    if(g_Deopt.depth_index == 0) {
        printf("Got end of struct\n");
        g_Deopt.currentState = DeoptState_ReadNextGlobalToken;
    }
}
void handle_read_script_tokens(QScriptToken *token) {
        g_Deopt.script_tokens.push_back(token);
        switch(token->GetType()) {
            case ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
            emit_script();
            g_Deopt.currentState = DeoptState_ReadNextGlobalToken;
            break;
        }
}

int main(int argc, const char *argv[]) {
    if(argc  < 3) {
        fprintf(stderr, "usage: %s [in] [out]\n",argv[0]);
        return -1;
    }
    g_Deopt.currentState = DeoptState_ReadNextGlobalToken;
    g_Deopt.depth_index = 0;
    FileStream fs(argv[1]);

    QStream qs = QStream(&fs);

    if(!fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return -1;
    }

    FileStream out_fs(argv[2], true);
    if(!out_fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[2]);
        return -1;
    }

    out_fs.SetWriteEndian(ISTREAM_BIG_ENDIAN);
    SymbolFileStream sym_fs(&out_fs);
    sym_fs.SetSourceChecksum(0xDEADBEEF);

    sym_fs.WriteHeader();

    g_Deopt.write_stream = reinterpret_cast<ISStream *>(&sym_fs);

    QScriptToken *token;
    while(true) {
        token = qs.NextToken();
        if(token == NULL || token->GetType() == ESCRIPTTOKEN_ENDOFFILE) {
            break;
        }
        switch(g_Deopt.currentState) {
            case DeoptState_ReadNextGlobalToken:
                handle_global_token_state(token);
                break;
            case DeoptState_ReadEqualsToken:
                handle_equals_token_state(token);
                break;
            case DeoptState_ReadTokenValue:
                handle_read_token_value(token);
                break;
            case DeoptState_ReadScriptName:
                handle_read_script_name(token);
                break;
            case DeoptState_ReadScriptTokens:
                handle_read_script_tokens(token);
                break;
            case DeoptState_ReadArrayTokens:
                handle_read_root_array(token);
                break;
            case DeoptState_ReadStructTokens:
                handle_read_root_struct(token);
                break;
        }
        //printf("token: %d\n", token->GetType());
    }

    sym_fs.UpdateHeaderSize();

    return 0;
}