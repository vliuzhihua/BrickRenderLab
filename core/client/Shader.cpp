#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

FShader* FShaderManager::CreateShader(const char* iVertexPath, const char* iFragmentPath) {
    FShader* Shader = new FShader(iVertexPath, iFragmentPath);
    mShaderVector.push_back(Shader);
    return Shader;
}

void FShaderManager::DeleteShader(FShader* iShader) {
    for (decltype(mShaderVector)::iterator Iter = mShaderVector.begin(); Iter != mShaderVector.end(); Iter++) {
        if (*Iter == iShader) {
            mShaderVector.erase(Iter);
            return;
        }
    }
}
void FShaderManager::InitDefaultShader(const char* iVecterPath, const char* iFragmentPath) {
    mDefaultShader = new FShader(iVecterPath, iFragmentPath);
}
void FShaderManager::UseDefaultShader() {
    if (mDefaultShader)
        mDefaultShader->Use();
}

FShader* FShaderManager::GetDefaultShader() {
    return mDefaultShader;
}

void FShaderManager::Update() {
    if (mNeedRecompiledAll) {
        std::cout << "All Shader Recompile" << std::endl;
        for (auto Shader : mShaderVector) {
            Shader->Recompile();
        }
        mNeedRecompiledAll = false;
    }
}

void FShaderManager::SetNeedRecompileAll(bool iState) {
    mNeedRecompiledAll = iState;
}

FShader::FShader(const char* iVertexPath, const char* iFragmentPath)
{
    Init(iVertexPath, iFragmentPath);
}

void FShader::Init(const char* iVertexPath, const char* iFragmentPath) {
    mVertexPath = iVertexPath;
    mFragmentPath = iFragmentPath;
    GLuint VertexShader, FragShader;

    if (!CreateShader(GL_VERTEX_SHADER, iVertexPath, VertexShader)) {
        std::cout << "Shader Create Failed " << iVertexPath << " :::: " << iFragmentPath << std::endl;
        return;
    }

    if (!CreateShader(GL_FRAGMENT_SHADER, iFragmentPath, FragShader)) {
        std::cout << "Shader Create Failed " << iVertexPath << " :::: " << iFragmentPath << std::endl;
        DeleteShader(VertexShader);
        return;
    }
    GLint Success;
    mID = glCreateProgram();
    glAttachShader(mID, VertexShader);
    glAttachShader(mID, FragShader);
    glLinkProgram(mID);

    glGetProgramiv(mID, GL_LINK_STATUS, &Success);
    if (!Success)
    {
        char InfoLog[512];
        glGetProgramInfoLog(mID, 512, NULL, InfoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(VertexShader);
    glDeleteShader(FragShader);
}

FShader::~FShader() {
    Release();
}

void FShader::Release() {
    if (mID != 0) {
        glDeleteProgram(mID);
        mID = 0;
    }
}

FShader* FShader::Use()
{
    if (IsInited()) {
        glUseProgram(mID);
        return this;
    }
    else {
        FShaderManager::Instance().UseDefaultShader();
        return FShaderManager::Instance().GetDefaultShader();
    }
}

void FShader::Recompile() {
    Release();
    Init(mVertexPath.c_str(), mFragmentPath.c_str());
}

void FShader::SetBool(const std::string& Name, const bool Value) const
{
    glUniform1i(glGetUniformLocation(mID, Name.c_str()), (int)Value);
}

void FShader::SetInt(const std::string& Name, const int Value) const
{
    glUniform1i(glGetUniformLocation(mID, Name.c_str()), Value);
}

void FShader::SetFloat(const std::string& Name, const float Value) const
{
    glUniform1f(glGetUniformLocation(mID, Name.c_str()), Value);
}

void FShader::SetFloat4(const std::string& Name, const float* Value) const {
    glUniform4f(glGetUniformLocation(mID, Name.c_str()), Value[0], Value[1], Value[2], Value[3]);
}

void FShader::SetMatrix4x4(const std::string& Name, const float* Value) const {
    glUniformMatrix4fv(glGetUniformLocation(mID, Name.c_str()), 1, true, Value);
}

void FShader::SetConstantBuffer(const std::string& Name, const int Value) const {
    glUniformBlockBinding(mID, glGetUniformLocation(mID, Name.c_str()), Value);
}

void FShader::SetDynamicBuffer(const std::string& Name, const int Value) const {
    glShaderStorageBlockBinding(mID, glGetUniformLocation(mID, Name.c_str()), Value);
}

bool GetFileContent(const char* Path, std::string& Content) {
    std::ifstream FileHandle;
    FileHandle.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        // 打开文件
        FileHandle.open(Path);
        std::stringstream vShaderStream, fShaderStream;
        // 读取文件的缓冲内容到数据流中
        vShaderStream << FileHandle.rdbuf();
        // 关闭文件处理器
        FileHandle.close();
        // 转换数据流到string
        Content = vShaderStream.str();
        return true;
    }
    catch (std::ifstream::failure e) {
        return false;
    }
}

std::vector<std::string> SplitString(const std::string S, std::string MatchC) {
    std::vector<std::string> Result;
    std::string::size_type pos1, pos2;
    pos2 = S.find(MatchC);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        Result.push_back(S.substr(pos1, pos2 - pos1 + MatchC.size()));

        pos1 = pos2 + MatchC.size();
        pos2 = S.find(MatchC, pos1);
    }
    if (pos1 != S.length())
        Result.push_back(S.substr(pos1));
    return Result;
}

std::string TrimStr(std::string str, const std::string& val)
{
    str.erase(0, str.find_first_not_of(val));
    str.erase(str.find_last_not_of(val) + val.size());
    return  str;
}

static bool FillShaderCode(const char* Path, std::string& Content) {
    //解析include语法
    std::string TmpCode;
    if (!GetFileContent(Path, TmpCode)) {
        std::cout << "File: " << Path << "Not Found!!" << std::endl;
        return false;
    }
    auto Lines = SplitString(TmpCode, "\n");
    for (auto Line : Lines) {
        std::string LineTrimed = TrimStr(Line, " ");
        size_t idx = LineTrimed.find("#include ");
        if (idx != -1) {
            std::string IncludePath = LineTrimed.substr(idx + 8);
            IncludePath = TrimStr(IncludePath, " ");
            IncludePath = TrimStr(IncludePath, "\n");
            IncludePath = TrimStr(IncludePath, "\"");
            //判断没有被注释
            if (LineTrimed[0] == '\\' || LineTrimed[1] == '\\') {
                Content += Line;
            }
            else {
                if (!FillShaderCode(IncludePath.c_str(), Content))
                    return false;
            }
        }
        else {
            Content += Line;
        }
    }
    return true;
}

bool FShader::CreateShader(GLuint ShaderType, const char* Path, GLuint& ShaderId) {
    std::string ShaderCode;
    if(!FillShaderCode(Path, ShaderCode))
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n " << "ShaderType: " << ShaderTypeName[ShaderType] << "\nPath: " << Path << std::endl;
        return false;
    }
    const char* vShaderCode = ShaderCode.c_str();

    // 2. 编译着色器
    int success;
    char infoLog[512];
    // 顶点着色器
    ShaderId = glCreateShader(ShaderType);
    glShaderSource(ShaderId, 1, &vShaderCode, NULL);
    glCompileShader(ShaderId);
    // 打印编译错误（如果有的话）
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(ShaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << "ShaderType:: " << ShaderTypeName[ShaderType] << "\nInfo:: " << infoLog << std::endl;
        return false;
    };
    return true;
}

void FShader::DeleteShader(GLuint ShaderId) {
    glDeleteShader(ShaderId);
}