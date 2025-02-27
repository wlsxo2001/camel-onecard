//
// Created by jintae on 25. 2. 12.
//

#ifndef NORMALCARD_HPP
#define NORMALCARD_HPP

#include "Card.hpp"

class NormalCard : public Card
{
private:
    std::string color;
    std::string shape;
    std::string value;
public:
    NormalCard(std::string c, std::string s, std::string v);
    void display() const override;
    std::string getType() const override;
    std::string getValue() const override;
    std::string getShape() const override;
    std::string getColor() const override;
    std::string getFullInfo() const override;
    int getAttackPower() const override;
    bool canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const override;
    bool useJQK() const override;
    void changeShape(const std::string& newShape) override;
    void changeColor(const std::string& newColor) override;
    bool change7(const std::shared_ptr<Card>& dummyCard) override;
};

#endif //NORMALCARD_HPP

