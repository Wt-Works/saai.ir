#include <string>

#include <Wt/WContainerWidget>

#include "div.hpp"

using namespace std;
using namespace Wt;
using namespace SAAIIR;

Div::Div(WContainerWidget *parent, const string& id) : WContainerWidget(parent) {
    setId(id);
}
