// 简单测试文件，验证模块语法是否正确
import std;
import registrar;
import :ui;

int main() {
    std::println("Testing module imports...");
    
    // 测试Registrar单例
    auto& registrar = Registrar::singleton();
    registrar.initialize();
    
    // 测试UI创建
    UI ui;
    std::println("All modules imported successfully!");
    
    return 0;
}