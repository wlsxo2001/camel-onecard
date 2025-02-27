#include "Card.hpp"

int Card::getAttackPower() const
{
    return 0;
}

bool Card::canDefend(const std::shared_ptr<Card>& attackCard, const std::shared_ptr<Card>& dummyCard) const
{
    return false;
}

bool Card::canPlayJoker(const std::string& dummyShape) const
{
    return false;
}

void Card::changeShape(const std::string& newShape)
{
}
void Card::changeColor(const std::string& newColor)
{
}

bool Card::change7(const std::shared_ptr<Card>& dummyCard)
{
}