#ifndef _ANS_AST_DEBUG_PRINT_H_
#define _ANS_AST_DEBUG_PRINT_H_
#include "parser_main_ans.h"
#define AST_ENUM_STR_OUT_POS 21
extern const char* debug_ans_ast_token_str[] = { Bulk_Gen_Ans_Parser_Token(CHANGE_TO_STR) };

#define ANS_AST_SPLIT_LR_TREE  "„¥„Ÿ„Ÿ "
#define ANS_AST_SPLIT_ONLY_TREE "„¤„Ÿ„Ÿ "
#define ANS_AST_ONLY_TREE     "„    "
#define ANS_AST_END_TREE    "    "

static void print_ast_tree_line(const Ans_Ast_Nodes* src_ans_node, const char* src_prefix, int last_check) {
	if (!src_ans_node) return;

	fputs(src_prefix, stdout);
	fputs(last_check ? ANS_AST_SPLIT_ONLY_TREE : ANS_AST_SPLIT_LR_TREE, stdout);
	Ans_Ast_Token_Type token_type = src_ans_node->token_type;
	if (src_ans_node->op) {
		if (token_type == E_Ans_Ast_Token_Type_Iden || token_type == E_Ans_Ast_Token_Type_Var_Decl ||
			token_type == E_Ans_Ast_Token_Type_Func_Decl || token_type == E_Ans_Ast_Token_Type_Func_Decl_Args ||
			token_type == E_Ans_Ast_Token_Type_Class_Decl) {
			printf("(\x1b[31m%s\x1b[0m)", (const char*)src_ans_node->op);
		}
		else if (token_type == E_Ans_Ast_Token_Type_String_Value) {
			if (*(const char*)src_ans_node->op == '\n')printf("(\x1b[31m\"^\\n\"\x1b[0m)");
			else printf("(\x1b[31m\"%s\"\x1b[0m)", (const char*)src_ans_node->op);
		}
		else if (token_type == E_Ans_Ast_Token_Type_Int_Value) {
			printf("(\x1b[35mint:%d\x1b[0m)", *(const int*)src_ans_node->op);
		}

		else if (token_type == E_Ans_Ast_Token_Type_Float_Value) {
			printf("(\x1b[35mfloat:%f\x1b[0m)", *(const float*)src_ans_node->op);
		}

		else if (token_type == E_Ans_Ast_Token_Type_Double_Value) {
			printf("(\x1b[35mdouble:%lf\x1b[0m)", *(const double*)src_ans_node->op);
		}
		else if (token_type == E_Ans_Ast_Token_Type_None_Value) {
			printf("(\x1b[34m\"%s\"\x1b[0m)", (const char*)src_ans_node->op);
		}
	}
	if (token_type == E_Ans_Ast_Token_Type_Abstract_Host) {
		printf("\x1b[33m%s\x1b[0m", debug_ans_ast_token_str[token_type] + AST_ENUM_STR_OUT_POS);
	}
	else printf("%s", debug_ans_ast_token_str[token_type] + AST_ENUM_STR_OUT_POS);
	putchar('\n');
	char next_prefix[512];
	snprintf(next_prefix, sizeof(next_prefix), "%s%s", src_prefix, last_check ? ANS_AST_END_TREE : ANS_AST_ONLY_TREE);

	if (token_type == E_Ans_Ast_Token_Type_Abstract_Host || token_type == E_Ans_Ast_Token_Type_Class_Filed_Public) {
		int j = 0;
		for (const Ans_Ast_Nodes* h = src_ans_node; h; h = h->left) j++;
		int i = 0;
		for (const Ans_Ast_Nodes* h = src_ans_node; h; h = h->left, ++i) print_ast_tree_line(h->right, next_prefix, (i == j - 1));

		return;
	}

	if (token_type == E_Ans_Ast_Token_Type_Func_Decl &&
		src_ans_node->left &&
		src_ans_node->left->token_type == E_Ans_Ast_Token_Type_Func_Decl_Args) {
		int cnt = 0;
		for (const Ans_Ast_Nodes* a = src_ans_node->left; a; a = a->left) cnt++;
		int i = 0;

		for (const Ans_Ast_Nodes* a = src_ans_node->left; a; a = a->left, ++i) {
			fputs(next_prefix, stdout);
			fputs((i == cnt - 1 && src_ans_node->right == NULL) ? ANS_AST_SPLIT_ONLY_TREE : ANS_AST_SPLIT_LR_TREE, stdout);
			Ans_Ast_Token_Type at = a->token_type;
			if (a->op) {
				if (at == E_Ans_Ast_Token_Type_Func_Decl_Args || at == E_Ans_Ast_Token_Type_Iden) {
					printf("(\x1b[31m%s\x1b[0m)", (const char*)a->op);
				}
				else if (at == E_Ans_Ast_Token_Type_String_Value) {
					printf("(\x1b[31m\"%s\"\x1b[0m)", (const char*)a->op);
				}

				else if (at == E_Ans_Ast_Token_Type_None_Value) {
					printf("(\x1b[34m\"%s\"\x1b[0m)", (const char*)a->op);
				}
			}
			printf("%s\n", debug_ans_ast_token_str[at] + AST_ENUM_STR_OUT_POS);
		}
		if (src_ans_node->right) {
			print_ast_tree_line(src_ans_node->right, next_prefix, 1);
		}
		return;
	}
	if (src_ans_node->left && src_ans_node->right) {
		print_ast_tree_line(src_ans_node->left, next_prefix, 0);
		print_ast_tree_line(src_ans_node->right, next_prefix, 1);
	}
	else if (src_ans_node->left) print_ast_tree_line(src_ans_node->left, next_prefix, 1);
	else print_ast_tree_line(src_ans_node->right, next_prefix, 1);
}
void print_ast_ans_node(const Ans_Ast_Nodes* src_ans_ast) {
	if (!src_ans_ast) { puts("(empty)"); return; }
	Ans_Ast_Token_Type token_type = src_ans_ast->token_type;

	if (token_type == E_Ans_Ast_Token_Type_Abstract_Host) {
		printf("\x1b[33m%s\x1b[0m\n", debug_ans_ast_token_str[token_type] + AST_ENUM_STR_OUT_POS);

		int j = 0; for (const Ans_Ast_Nodes* h = src_ans_ast; h; h = h->left) j++;
		int i = 0;   for (const Ans_Ast_Nodes* h = src_ans_ast; h; h = h->left, ++i) print_ast_tree_line(h->right, "", (i == j - 1));
	}
	else {
		printf("error : not find to begin abstract node\n");
		return;
	}
}
void normal_debug_ans_ast_print(Ans_Ast_Nodes* src_ans_ast_nodes, uint32_t src_idx) {
	if (!src_ans_ast_nodes)return;
	normal_debug_ans_ast_print(src_ans_ast_nodes->left, src_idx + 1);
	for (uint32_t i = 0; i < src_idx; ++i)putchar('.');
	printf("%s", debug_ans_ast_token_str[src_ans_ast_nodes->token_type] + AST_ENUM_STR_OUT_POS);
	if (src_ans_ast_nodes->op) {
		if (src_ans_ast_nodes->token_type == E_Ans_Ast_Token_Type_Iden) printf("(%s)", (char*)src_ans_ast_nodes->op);
		if (src_ans_ast_nodes->token_type == E_Ans_Ast_Token_Type_Var_Decl) printf("(%s)", (char*)src_ans_ast_nodes->op);
		else if (src_ans_ast_nodes->token_type == E_Ans_Ast_Token_Type_Int_Value) printf("(%d)", *(int*)src_ans_ast_nodes->op);
		else if (src_ans_ast_nodes->token_type == E_Ans_Ast_Token_Type_None_Value) printf("(%s)", (char*)src_ans_ast_nodes->op);
	}
	putchar('\n');
	normal_debug_ans_ast_print(src_ans_ast_nodes->right, src_idx + 1);
}
#endif // !_ANS_AST_DEBUG_PRINT_H_
