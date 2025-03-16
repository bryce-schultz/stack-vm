#include "LithiumTokenizer.h"
#include "Util.h"

int main()
{
    std::string source = Util::readWholeFile("test.li");
    LithiumTokenizer tokenizer(source, "test.li");

    while(!tokenizer.eof())
    {
        Token token = tokenizer.nextToken();
        if (token == END)
        {
            break;
        }

        std::string text = token.getText();

        if (token == STRING)
        {
            text = "\"" + text + "\"";
        }
        else if (token == JUNK)
        {
            text = "junk(" + text + ")";
        }

        printf("%d: %s:%ld:%ld: %s\n", token.getType(), token.getLocation().getFilename().c_str(), token.getLocation().getLine(), token.getLocation().getColumn(), text.c_str());
    }

    return 0;
}