#include <stdio.h>
#include <ctype.h>
char peekc(FILE * fp)
{
    char ch = fgetc(fp);
    ungetc(ch,fp);
    return ch;
}
void parse_word(char nChar, FILE * fp, FILE * wfp)
{
    nChar = peekc(fp);
    while((nChar >= 65 && nChar <= 90) || nChar == 95 || (nChar >= 97 && nChar <= 122) )
    {
	fprintf(wfp, "%c", nChar);
        fgetc(fp);
        nChar = peekc(fp);
    }
}
void parse_comment(char inch, FILE * fp, FILE * wfp)
{
    char next = peekc(fp);
    while( ! (inch == '*' && next == '/') )
    {
        inch = fgetc(fp);
	next = peekc(fp);
    }
    fgetc(fp);
}
void parse_single_line_comment(char inch, FILE * fp, FILE * wfp)
{
    char next = peekc(fp);
    while( next != 10  )
    {
        inch = fgetc(fp);
	next = peekc(fp);
    }
    fgetc(fp);
}
void decide_kind_of_comment(char inch, FILE * fp, FILE * wfp)
{
    char temp = peekc(fp);
    if( temp == 47  )
        parse_single_line_comment(inch, fp,  wfp);
    else if ( temp == '*')
	parse_comment(inch, fp, wfp);
}
void parse_operator(char inch, FILE * fp, FILE * wfp)
{
        char temp;        
	switch(inch)
	{
	    case '+':
		temp = peekc(fp);
		if (temp == '+' || temp == '=')
		    fprintf(wfp,"%c", fgetc(fp));
		break;
	    case '-':
		temp = peekc(fp);
		if (temp == '-' || temp == '>' || temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	    case '*':
		temp = peekc(fp);
		if (temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	    case '=':
		temp = peekc(fp);
		if (temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	    case '&':
		temp = peekc(fp);
		if (temp == '&' || temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	    case '|':
		temp = peekc(fp);
		if(temp == '|' || temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	    case '^':
		temp = peekc(fp);
		if(temp == '=')
		    fprintf(wfp,"%c", fgetc(fp));
		break;
	    case '%':
		temp = peekc(fp);
		if(temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	    case '!':
		temp = peekc(fp);
		if(temp == '=')
		    fprintf(wfp,"%c", fgetc(fp));
		break;
	    case '>':
		temp = peekc(fp);
		if(temp == '>' || temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	    case '<':
		temp = peekc(fp);
		if (temp == '<' || temp == '=')
		    fprintf(wfp, "%c", fgetc(fp));
		break;
	}
}
void print_not_comment_op(char inch, char temp, FILE * fp, FILE * wfp)
{
    fprintf(wfp, "%c", inch);
    if ( temp == '='  )
	fprintf(wfp, "%c", fgetc(fp));
    fprintf(wfp, "\n");
    
}
void parse_slash(char inch, FILE * fp, FILE * wfp)
{
    char temp = peekc(fp);
    if(temp == '*' || temp == 47)
    {
	decide_kind_of_comment(inch,fp,wfp);
    }
    else
    {
        print_not_comment_op(inch,temp,fp,wfp);
    }
}
void parse_char_lit(char inch, FILE * fp, FILE * wfp)
{
    char temp = fgetc(fp);
    while(temp != 39)
    {
        fprintf(wfp, "%c", temp);
        temp = fgetc(fp);
    }
}
void parse_string_lit(char inch, FILE * fp, FILE * wfp)
{
    char temp = fgetc(fp);
    while(temp != '"' || ( inch == 92 ))
    {
        fprintf(wfp, "%c", temp);
        inch = temp;
        temp = fgetc(fp);
    }
}
void parse_num(char inch, FILE * fp, FILE * wfp)
{
    fprintf(wfp, "%c", inch);
    if( isdigit(inch = peekc(fp)) )
    {
        fgetc(fp);
	parse_num(inch,fp,wfp);
    }
}
void is_alpha_case(char inch, FILE * fp, FILE * wfp )
{
    fprintf(wfp, "%c", inch);
    parse_word(inch, fp, wfp);
    fprintf(wfp, "\n");
}
void getToken(FILE * fp, FILE * wfp)
{
    char inch;
    while( (inch = fgetc(fp)) != EOF  )
    {
        switch(inch)
        {
	    case ' ':
	    case '\t':
	    case '\n':
		break;
	    case '+':
	    case '-':
	    case '*':
	    case '=':
	    case '&':
	    case '|':
	    case '^':
	    case '%':
	    case '!':
	    case '>':
	    case '<':
                fprintf(wfp, "%c", inch);
		parse_operator(inch,fp, wfp);
		fprintf(wfp,"\n");
		break;
            case '/':
		parse_slash(inch,fp,wfp);
		break;
	    case '~':
	    case '#':
	    case '[':
	    case ']':
	    case '{':
	    case '}':
	    case ';':
	    case ':':
            case '(':
	    case ')':
	    case '?':
	    case '.':
	        fprintf(wfp, "%c\n", inch);
		break;
            case 39:
                fprintf(wfp, "%c", inch);
		parse_char_lit(inch,fp,wfp);
		fprintf(wfp, "%c\n", inch);
		break;
            case '"':
		fprintf(wfp, "%c", inch);
		parse_string_lit(inch,fp,wfp);
                fprintf(wfp, "%c\n", inch);
		break;
            default:
		if (isdigit(inch))
		    {
		        parse_num(inch,fp,wfp);
			fprintf(wfp,"\n");
		    }
	        else if (isalpha(inch))
		    is_alpha_case(inch,fp,wfp);
        }
    }
}

int main()
{
    FILE * fp = stdin;
    FILE * wfp = stdout;
    getToken(fp, wfp);
    return 0;
}
