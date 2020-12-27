#pragma once

#include <glad/glad.h>

#include <string>
#include <map>
#include <vector>

#define SHADER_MAP_PARI(ShaderType) {ShaderType, #ShaderType}

class FShader
{
public:
    ~FShader();
    bool IsInited() { return mID != 0; };
    void SetInited(bool State);
    // active or not, 实际使用可能会有当前shader编译不过，替换的情况，返回替换的shader
    FShader* Use();
    void Recompile();
    // uniform utility function
    void SetBool(const std::string& Name, const bool Value) const;
    void SetInt(const std::string& Name, const int Value) const;
    void SetFloat(const std::string& Name, const float Value) const;
    void SetFloat4(const std::string& Name, const float* Value) const;
    void SetMatrix4x4(const std::string& Name, const float* Value) const;
   
protected:
    // bulid shader, ony called by FShaderManager
    FShader(const char* iVertexPath, const char* iFragmentPath);
    void Release();
    void Init(const char* iVertexPath, const char* iFragmentPath);

private:
    bool CreateShader(GLuint ShaderType, const char* Path, GLuint& ShaderId);
    void DeleteShader(GLuint Shader);
    std::string mVertexPath, mFragmentPath;

    //gl shader program id
    unsigned int mID = 0;


    std::map<GLuint, std::string> ShaderTypeName = {
        SHADER_MAP_PARI(GL_VERTEX_SHADER),
        SHADER_MAP_PARI(GL_FRAGMENT_SHADER),
        SHADER_MAP_PARI(GL_GEOMETRY_SHADER),
        SHADER_MAP_PARI(GL_TESS_CONTROL_SHADER),
        SHADER_MAP_PARI(GL_COMPUTE_SHADER)
    };

    friend class FShaderManager;
};

class FShaderManager
{
public:
    static FShaderManager& Instance() {
        static FShaderManager ShaderManager;
        return ShaderManager;
    }
    FShader* CreateShader(const char* iVertexPath, const char* iFragmentPath);
    void DeleteShader(FShader* iShader);
    void Update();
    void SetNeedRecompileAll(bool iState);

    //之后可能依据不同类型有不同的替换函数？
    void InitDefaultShader(const char* iVecterPath, const char* iFragmentPath);
    void UseDefaultShader();
    FShader* GetDefaultShader();
private:
    FShaderManager() {};
    std::vector<FShader*> mShaderVector;
    bool mNeedRecompiledAll;
    FShader* mDefaultShader = nullptr;

};

