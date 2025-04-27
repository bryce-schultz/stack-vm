const vscode = require('vscode');
const hoverProvider = require('./hoverProvider');

function activate(context) {
    hoverProvider(context);
}

function deactivate() {
}

module.exports = {
    activate,
    deactivate
};