/*
| ==============================================================================
| Copyright (C) 2012 Allied Vision Technologies.  All Rights Reserved.
|
| This code may be used in part, or in whole for your application development.
|
|==============================================================================
|
| THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
| WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
| NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
| DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
| INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
| LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
| OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF
| LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
| NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
| EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
|
| http://code.google.com/p/qhexedit2/
|==============================================================================
*/

#include "Commands.h"

CharCommand::CharCommand(XByteArray * xData, Cmd cmd, int charPos, char newChar, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    _xData = xData;
    _charPos = charPos;
    _newChar = newChar;
    _cmd = cmd;
}

bool CharCommand::mergeWith(const QUndoCommand *command)
{
    const CharCommand *nextCommand = static_cast<const CharCommand *>(command);
    bool result = false;

    if (_cmd != remove)
    {
        if (nextCommand->_cmd == replace)
            if (nextCommand->_charPos == _charPos)
            {
                _newChar = nextCommand->_newChar;
                result = true;
            }
    }
    return result;
}

void CharCommand::undo()
{
    switch (_cmd)
    {
        case insert:
            _xData->remove(_charPos, 1);
            break;
        case replace:
            _xData->replace(_charPos, _oldChar);
            _xData->setDataChanged(_charPos, _wasChanged);
            break;
        case remove:
            _xData->insert(_charPos, _oldChar);
            _xData->setDataChanged(_charPos, _wasChanged);
            break;
    }
}

void CharCommand::redo()
{
    switch (_cmd)
    {
        case insert:
            _xData->insert(_charPos, _newChar);
            break;
        case replace:
            _oldChar = _xData->data()[_charPos];
            _wasChanged = _xData->dataChanged(_charPos);
            _xData->replace(_charPos, _newChar);
            break;
        case remove:
            _oldChar = _xData->data()[_charPos];
            _wasChanged = _xData->dataChanged(_charPos);
            _xData->remove(_charPos, 1);
            break;
    }
}



ArrayCommand::ArrayCommand(XByteArray * xData, Cmd cmd, int baPos, QByteArray newBa, int len, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    _cmd = cmd;
    _xData = xData;
    _baPos = baPos;
    _newBa = newBa;
    _len = len;
}

void ArrayCommand::undo()
{
    switch (_cmd)
    {
        case insert:
            _xData->remove(_baPos, _newBa.length());
            break;
        case replace:
            _xData->replace(_baPos, _oldBa);
            _xData->setDataChanged(_baPos, _wasChanged);
            break;
        case remove:
            _xData->insert(_baPos, _oldBa);
            _xData->setDataChanged(_baPos, _wasChanged);
            break;
    }
}

void ArrayCommand::redo()
{
    switch (_cmd)
    {
        case insert:
            _xData->insert(_baPos, _newBa);
            break;
        case replace:
            _oldBa = _xData->data().mid(_baPos, _len);
            _wasChanged = _xData->dataChanged(_baPos, _len);
            _xData->replace(_baPos, _newBa);
            break;
        case remove:
            _oldBa = _xData->data().mid(_baPos, _len);
            _wasChanged = _xData->dataChanged(_baPos, _len);
            _xData->remove(_baPos, _len);
            break;
    }
}
