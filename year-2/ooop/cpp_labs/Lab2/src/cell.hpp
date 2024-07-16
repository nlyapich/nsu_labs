#ifndef CELL_HPP
#define CELL_HPP

namespace gol
{
  class Cell
  {
  public:
    Cell();
    Cell(bool isActive);
    virtual ~Cell();

    bool isActive() const;
    void setIsActive(bool isActive);

  private:
    bool _isActive;

  };
} // namespace gol

#endif // CELL_HPP
