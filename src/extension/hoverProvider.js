const vscode = require('vscode');

const hoverTexts = {
    "print": "Outputs the specified text or value to the console."
};

function provideHover(document, position, token) {
    const range = document.getWordRangeAtPosition(position);
    const word = document.getText(range);

    if (hoverTexts[word]) {
        return new vscode.Hover(hoverTexts[word]);
    }
}

module.exports = function(context) {
    context.subscriptions.push(vscode.languages.registerHoverProvider('lithium', {
        provideHover
    }));
};