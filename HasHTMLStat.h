// HasHTMLStat.h
#ifndef HAS_HTML_STAT_H
#define HAS_HTML_STAT_H

#include <String>

class HasHTMLStat {
public:
    virtual ~HasHTMLStat() = default;
    virtual String getHTMLStat() const = 0;
};

#endif // HAS_HTML_STAT_H
