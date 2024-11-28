#include "Equipment.h"

// 构造函数
Equipment::Equipment(const std::string& name, int power)
    : name(name), power(power) {}

// 获取装备名称
const std::string& Equipment::getName() const {
    return name;
}

// 获取装备的属性（攻击力或防御力）
int Equipment::getPower() const {
    return power;
}

// 打印装备信息（虚拟函数，允许子类重写）
void Equipment::printInfo() const {
    CCLOG("Equipment Name: %s", name.c_str());
    CCLOG("Power: %d", power);
}


// 将装备数据序列化为 JSON 字符串
std::string Equipment::saveToJson() const {
    rapidjson::Document doc;
    doc.SetObject();
    auto& allocator = doc.GetAllocator();

    // 将装备的成员变量序列化为 JSON 对象
    doc.AddMember("name", rapidjson::Value(name.c_str(), allocator), allocator);
    doc.AddMember("power", power, allocator);

    // 转换为 JSON 字符串
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    return buffer.GetString();
}

// 从 JSON 字符串加载装备数据
void Equipment::loadFromJson(const std::string& jsonString) {
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());

    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("Error parsing JSON");
        return;
    }

    // 从 JSON 中加载装备数据
    if (doc.HasMember("name")) name = doc["name"].GetString();
    if (doc.HasMember("power")) power = doc["power"].GetInt();
}

// 将成员变量序列化为 JSON 格式，并保存到本地
void Entities::saveToFile(const std::string& filePath) const {
    std::string jsonString = saveToJson(); // 调用 saveToJson 获取 JSON 字符串
    std::ofstream file(filePath);
    if (file.is_open()) {
        file << jsonString;
        file.close();
        CCLOG("Entity saved to file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}

// 从本地读取 JSON 文件，读取成员变量序列
void Entities::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::stringstream buffer;
        buffer << file.rdbuf(); // 读取整个文件内容到 buffer
        file.close();
        std::string jsonString = buffer.str();
        loadFromJson(jsonString); // 调用 loadFromJson 解析 JSON 数据
        CCLOG("Entity loaded from file: %s", filePath.c_str());
    }
    else {
        CCLOG("Failed to open file: %s", filePath.c_str());
    }
}