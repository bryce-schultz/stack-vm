const vscode = require('vscode');
const hoverProvider = require('./hoverProvider');

function activate(context) {
    console.log('Congratulations, your extension "hover-provider" is now active!');
    hoverProvider(context);
}

function deactivate() {
    // Cleanup code...
}

module.exports = {
    activate,
    deactivate
};