/**
 * Copyright (C) 2013 - 2019 : Kathrin Hanauer
 *
 * This file is part of Algora.
 *
 * Algora is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Algora is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Algora.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact information:
 *   http://algora.xaikal.org
 */

#ifndef GRAPHARTIFACT_H
#define GRAPHARTIFACT_H

#include <string>

namespace Algora {

class GraphArtifact
{
public:
    typedef std::size_t size_type;
    typedef std::size_t id_type;

    explicit GraphArtifact(id_type id, GraphArtifact *parent = nullptr);
    explicit GraphArtifact(GraphArtifact *parent = nullptr);
    virtual ~GraphArtifact();

    // copying
    GraphArtifact(const GraphArtifact &other);
    GraphArtifact& operator=(const GraphArtifact &other);

    // moving
    GraphArtifact(GraphArtifact &&other) = default;
    GraphArtifact& operator=(GraphArtifact &&other) = default;

    id_type getId() const { return id; }
    GraphArtifact *getParent() const { return parent; }

    virtual std::string typeString() const noexcept {
        return "GraphArtifact";
    }
    virtual std::string toString() const = 0;

    bool isValid() const { return valid; }

    void setName(const std::string &n) {
        name = n;
    }
    const std::string &getName() const {
        return name;
    }

    // needed to implement move semantics in graph classes
    virtual void setParent(GraphArtifact *p) {
       parent = p;
    }

protected:
    std::string idString() const;
    void invalidate() { valid = false; }
    void revalidate() { valid = true; }

private:
    static id_type nextId;

    id_type id;
    GraphArtifact *parent;
    bool valid;
    std::string name;
};

}

#endif // GRAPHARTIFACT_H
