struct UIEvent {
    enum Type {
        MOUSE_MOVE,
        MOUSE_PRESS,
        MOUSE_RELEASE,
        KEY_PRESS,
        KEY_RELEASE,
        CHAR_INPUT  // 添加字符输入事件
    };
    
    Type type;
    double mouseX, mouseY;
    int mouseButton;
    int keyCode;
    int modifiers;
    unsigned int character;  // 添加字符字段
};