#include <vector>
#include <string>
#include <sstream>

class ContextInjector {
private:
    struct KnowledgeChunk {
        std::string source;
        std::string content;
        float relevance;
    };

    std::vector<KnowledgeChunk> context_cache;
    const std::string prompt_template = 
        u8"基于以下知识库（相关性降序）：\n"
        u8"{context_str}\n"
        u8"请以{role}的身份回答：{query}\n"
        u8"要求：{format}";

public:
    // 添加上下文缓存
    void add_context(const std::string& src, 
                    const std::string& cnt, 
                    float rel) {
        context_cache.push_back({src, cnt, rel});
        sort_context();  // 按相关性排序
    }

    // 动态生成prompt
    std::string generate_prompt(const std::string& role,
                               const std::string& query,
                               const std::string& format) {
        std::ostringstream context_stream;
        for (const auto& chunk : context_cache) {
            context_stream << "[" << chunk.source << "]\n"
                         << chunk.content << "\n\n";
        }

        // 模板替换
        std::string prompt = prompt_template;
        replace_all(prompt, "{context_str}", context_stream.str());
        replace_all(prompt, "{role}", role);
        replace_all(prompt, "{query}", query);
        replace_all(prompt, "{format}", format);

        return prompt;
    }

private:
    // 字符串替换工具
    void replace_all(std::string& str, 
                    const std::string& from, 
                    const std::string& to) {
        size_t pos = 0;
        while((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }

    // 按相关性排序
    void sort_context() {
        std::sort(context_cache.begin(), context_cache.end(),
            [](const KnowledgeChunk& a, const KnowledgeChunk& b) {
                return a.relevance > b.relevance;
            });
    }
};
