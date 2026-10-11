#include "main/script/sys/ScriptTree.hpp"

bool (*ScriptTree::executeFunction_)(int);
bool (*ScriptTree::displayFunction_)(int);
bool (*ScriptTree::getScriptCommandClearFlag_)(int);
void (*ScriptTree::clearStatusFunction_)(int);
int (*ScriptTree::getScriptCommandTypeFunction_)(int);
bool (*ScriptTree::checkStatusFunction_)();

ARM ScriptTree::ScriptTree()
{
}

ARM ScriptTree::~ScriptTree()
{
}

ARM void ScriptTree::setExecuteFunction(bool (*fc)(int))
{
    executeFunction_ = fc;
}

ARM void ScriptTree::setCheckStatusFunction(bool (*fc)())
{
    checkStatusFunction_ = fc;
}

ARM void ScriptTree::setClearStatusFunction(void (*fc)(int))
{
    clearStatusFunction_ = fc;
}

ARM void ScriptTree::setGetScriptCommandTypeFunction(int (*fc)(int))
{
    getScriptCommandTypeFunction_ = fc;
}

ARM void ScriptTree::setGetScriptCommandClearFlag(bool (*fc)(int))
{
    getScriptCommandClearFlag_ = fc;
}

ARM void ScriptTree::setDisplayFunction(bool (*fc)(int))
{
    displayFunction_ = fc;
}

ARM void ScriptTree::recursiveTree()
{
    while (true) {
        char cmdIndex = getCurrentNode()->getValue();
        if (cmdIndex == -1) {
            if (isCurrentChild()) {
                moveCurrentChild();
                recursiveTree();
                moveCurrentParent(1);
            }
            return;
        }
        bool execFlag = executeFunction_(cmdIndex);
        bool endFlag = checkStatusFunction_();
        if (execFlag && isCurrentChild()) {
            moveCurrentChild();
            recursiveTree();
            moveCurrentParent(1);
        }
        if (!endFlag) {
            return;
        }
        if (isCurrentNext()) {
            moveCurrentNext();
            continue;
        }
        if (!getScriptCommandClearFlag_(getCurrentNode()->getValue())) {
            return;
        }
        while (isCurrentPrev()) {
            moveCurrentPrev();
        }
        while (isCurrentNext()) {
            clearStatusFunction_(getCurrentNode()->getValue());
            moveCurrentNext();
        }
        clearStatusFunction_(getCurrentNode()->getValue());
        return;
    }
}

ARM void ScriptTree::display()
{
    moveCurrentRoot();
    recursiveDisplay();
}

ARM void ScriptTree::recursiveDisplay()
{
    while (true) {
        executeFunction_(getCurrentNode()->getValue());
        if (isCurrentChild()) {
            moveCurrentChild();
            recursiveDisplay();
            moveCurrentParent(1);
        }
        if (!isCurrentNext()) {
            return;
        }
        moveCurrentNext();
    }
}

template dss::Tree<char, 128>::Tree();
template void dss::Tree<char, 128>::moveCurrentRoot();
template void dss::Tree<char, 128>::moveCurrentChild();
template void dss::Tree<char, 128>::moveCurrentNext();
template void dss::Tree<char, 128>::moveCurrentPrev();
template void dss::Tree<char, 128>::moveCurrentParent(int);
template bool dss::Tree<char, 128>::isCurrentChild();
template bool dss::Tree<char, 128>::isCurrentNext();
template bool dss::Tree<char, 128>::isCurrentPrev();
template dss::Node<char>* dss::Tree<char, 128>::getCurrentNode();
template dss::NodeArray<char, 128>::NodeArray();
template dss::Node<char>::Node();
template char dss::Node<char>::getValue();
template void dss::Tree<char, 128>::display();
template void dss::Tree<char, 128>::recursiveTree();
