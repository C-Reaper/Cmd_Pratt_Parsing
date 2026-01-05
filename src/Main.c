#include "/home/codeleaded/System/Static/Library/AST.h"
#include "/home/codeleaded/System/Static/Library/Files.h"

#define SRC_PATH            "./code/Main"

#define TOKEN_LINECOMMENT   (TOKEN_START + 0)
#define TOKEN_ASS           (TOKEN_START + 1)
#define TOKEN_ADD           (TOKEN_START + 2)
#define TOKEN_SUB           (TOKEN_START + 3)
#define TOKEN_MUL           (TOKEN_START + 4)
#define TOKEN_DIV           (TOKEN_START + 5)

Token Ass_Handler(void* parent,Token* op,TokenMap* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return Token_Move(TOKEN_NUMBER,Number_Get(Number_Parse(b->str)));
}
Token Add_Handler(void* parent,Token* op,TokenMap* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return Token_Move(TOKEN_NUMBER,Number_Get(Number_Parse(a->str) + Number_Parse(b->str)));
}
Token Sub_Handler(void* parent,Token* op,TokenMap* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return Token_Move(TOKEN_NUMBER,Number_Get(Number_Parse(a->str) - Number_Parse(b->str)));
}
Token Mul_Handler(void* parent,Token* op,TokenMap* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    return Token_Move(TOKEN_NUMBER,Number_Get(Number_Parse(a->str) * Number_Parse(b->str)));
}
Token Div_Handler(void* parent,Token* op,TokenMap* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);
    
    Number nb = Number_Parse(b->str);
    if(nb == 0){
        printf("[Div_Handler]: Division by Zero!\n");
        return Token_Move(TOKEN_NUMBER,Number_Get(nb));
    }
    return Token_Move(TOKEN_NUMBER,Number_Get(Number_Parse(a->str) / nb));
}

int main(int argc, const char *argv[]){
    Parser p = Parser_New();
    
    FilesSize size;
    TT_Char* data = Files_ReadTB(SRC_PATH,&size);
    Parser_Parse_CStr(&p,data,SRC_PATH);
    Parser_TF_Num(&p);
    Parser_TF_Esc(&p);
    Parser_TF_Std(&p);


    KeywordMap km = KeywordMap_Make((KeywordRP[]){
        //KeywordRP_New("let",TOKEN_LET),
        KeywordRP_Null()
    });
    Parser_TF_KW(&p,&km);
    KeywordMap_Free(&km);


    ReseterMap rm = ReseterMap_Std();
    OperatorMap om = OperatorMap_Make((OperatorRP[]){
        OperatorRP_Make((TT_Type[]){ TOKEN_EQUAL_SIGN,TOKEN_END },       TOKEN_ASS,ARGS_IGNORE),
        OperatorRP_Make((TT_Type[]){ TOKEN_PLUS_SIGN,TOKEN_END },        TOKEN_ADD,ARGS_IGNORE),
        OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_END },       TOKEN_SUB,ARGS_IGNORE),
        OperatorRP_Make((TT_Type[]){ TOKEN_ASTERISK,TOKEN_END },         TOKEN_MUL,ARGS_IGNORE),
        OperatorRP_Make((TT_Type[]){ TOKEN_SLASH,TOKEN_END },            TOKEN_DIV,ARGS_IGNORE),
        OperatorRP_Make((TT_Type[]){ TOKEN_SLASH,TOKEN_SLASH,TOKEN_END },TOKEN_LINECOMMENT,ARGS_IGNORE),
        OperatorRP_End()
    });
    Parser_TF_OPS(&p,&om,&rm);
    OperatorMap_Free(&om);
    ReseterMap_Free(&rm);


    DTT_TypeMap dtm = DTT_TypeMap_Make((DTT_Type[]){
        DTT_Type_New(TOKEN_NEWLINE,Token_Null()),
        DTT_Type_Null()
    });
    ShutingYard sy = ShutingYard_New(
        PrecedenceMap_Make((Precedencer[]){
            //Precedencer_New(TOKEN_LET,1),
            Precedencer_New(TOKEN_ASS,2),
            Precedencer_New(TOKEN_ADD,4),
            Precedencer_New(TOKEN_SUB,3),
            Precedencer_New(TOKEN_MUL,5),
            Precedencer_New(TOKEN_DIV,6),
            Precedencer_New(TOKEN_PARENTHESES_L,PRECEDENCE_BRACKL),
            Precedencer_New(TOKEN_PARENTHESES_R,PRECEDENCE_BRACKR),
            Precedencer_End
        }),
        ExecuteMap_Make((Executer[]){
            //Executer_New(TOKEN_LET,1,Let_Handler),
            Executer_New(TOKEN_ASS,2,Ass_Handler),
            Executer_New(TOKEN_ADD,2,Add_Handler),
            Executer_New(TOKEN_SUB,2,Sub_Handler),
            Executer_New(TOKEN_MUL,2,Mul_Handler),
            Executer_New(TOKEN_DIV,2,Div_Handler),
            Executer_End
        }),
        PreexecuteMap_Make((Preexecuter[]){
            Preexecuter_End
        })
    );
    
    Vector tms = Parser_Split_C(&p,&dtm);
    for(int i = 0;i<tms.size;i++){
        TokenMap* tm = (TokenMap*)Vector_Get(&tms,i);
        TokenMap sytm = ShutingYard_Transform(&sy,tm);
        
        //TokenMap_Print(&sytm);

        AST_Pratt(&sytm,&sy);
        
        Token* tok = (Token*)Vector_Get(&sytm,0);
        //Token_Print(tok);
        
        Token ret = AST_Execute(NULL,tok,&sy);
        Token_Print(&ret);
        Token_Free(&ret);
        
        //TokenMap_Print(&sytm);
        
        TokenMap_Free(&sytm);
        TokenMap_Free(tm);
    }
    Vector_Free(&tms);

    ShutingYard_Free(&sy);
    DTT_TypeMap_Free(&dtm);


    //Parser_Print(&p);
    Parser_Free(&p);
    return 0;
}