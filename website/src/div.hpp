#ifndef DIV_HPP
#define DIV_HPP


#include <string>

#include <Wt/WContainerWidget>

namespace SAAIIR {
    class Div;
}

class SAAIIR::Div : public Wt::WContainerWidget {
public:
    Div(Wt::WContainerWidget *parent, const std::string& id);
};


#endif /* DIV_HPP */
