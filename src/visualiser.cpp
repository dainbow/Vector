#include "../include/microscope.hpp"
#include "../include/visualiser.hpp"

Visualiser* Visualiser::instance = nullptr;

Visualiser::Visualiser(const std::string& outputName) :
    m_nodesAmount(0),
    m_tmpAmount(0), m_funcAmount(0),
    m_copyCtorAmount(0), m_moveCtorAmount(0), m_copyEqAmount(0), m_moveEqAmount(0),
    m_file(outputName),
    m_enableLogging(true)
{
    m_file << "digraph structs {\n\trankdir=HR; outputOrder=nodesfirst;\n";
}

Visualiser::~Visualiser() {
    m_file << "label = \"temporary objects amount is " << m_tmpAmount << ",\n"
           << "copy constructors amount is " << m_copyCtorAmount << ",\n"
           << "move constructors amount is " << m_moveCtorAmount << ",\n"
           << "copy equations amount is " << m_copyEqAmount << ",\n"
           << "move equations amount is " << m_moveEqAmount << "\"\n";
    m_file << "}\n";

    m_file.close();
}

Visualiser& Visualiser::GetInstance(const std::string& outputName) {
    if (instance == nullptr) {
        instance = new Visualiser(outputName);
    }

    return *instance;
}

void Visualiser::RemoveInstance() {
    if (instance) {
        delete instance;
    }

    instance = nullptr;
}

void Visualiser::DisableLogging() {
    m_enableLogging = false;
}

void Visualiser::EnableLogging() {
    m_enableLogging = true;
}

void Visualiser::LogFuncEntry(const std::string& funcName) {
    if (!m_enableLogging) {
        return;
    }

    m_file << "subgraph cluster_" << m_funcAmount++ << " {\n";
    m_file << "label = \"" << funcName << "\"\n";
    m_file << "bgcolor = \"#b0b0ff40\"\n";
}

void Visualiser::LogFuncExit() {
    if (!m_enableLogging) {
        return;
    }

    m_file << "}\n";
}

std::string Visualiser::CreateCtorNode(const std::string& name, const std::string& color) {
    std::string nodeName = std::to_string(m_nodesAmount++);

    m_file << nodeName 
           << " [style = filled label = \"" << name << "\" shape = \"doublecircle\" fillcolor = \"" 
           << color << "\"]\n";

    return nodeName;
}

std::string Visualiser::CreateOperatorNode(const std::string& name, const std::string& color) {
    std::string nodeName = std::to_string(m_nodesAmount++);

    m_file << nodeName 
           << " [style = filled label = \"" << name << "\" shape = \"ellipse\" fillcolor = \"" 
           << color << "\"]\n";

    return nodeName;
}

void Visualiser::CreateEdge(const std::string& from, const std::string& to, const std::string& label, const std::string& style) {
    m_file << from << "->" << to << " [style =\"" << style << "\", label=\"" << label << "\"]\n";
}

void Visualiser::ProcessCtorType(const TransferType& type, std::string& ctorName, std::string& ctorColor) {
    if (type == TransferType::Move) {
        ctorName = "move ctor";
        ctorColor = GREEN;
        m_moveCtorAmount++;

        return;
    }
    
    ctorName = "copy ctor";
    ctorColor = RED;
    m_copyCtorAmount++;
}

void Visualiser::ProcessEqType(const TransferType& type, std::string& eqColor) {
    if (type == TransferType::Move) {
        eqColor = GREEN;
        m_moveEqAmount++;

        return;
    }

    eqColor = RED;
    m_copyEqAmount++;
}

FuncLogger::FuncLogger(const std::string& functionName) {
    Visualiser::GetInstance().LogFuncEntry(functionName);
}

FuncLogger::~FuncLogger() {
    Visualiser::GetInstance().LogFuncExit();
}
