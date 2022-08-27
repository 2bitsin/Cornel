#include <cstdint>

#include <interrupts.hpp>
#include <globals.hpp>
#include <macros.hpp>
#include <display.hpp>

CO_PUBLIC 
CO_SECTION(".text.main")
auto main () -> void
{
  using namespace std::string_view_literals;

  global_initialize();  
  display_write({
    "+-----------------------+-------------------+\n"sv,
    "| Country               | Population count  |\n"sv,
    "+-----------------------+-------------------+\n"sv,
    "| 1.  China             |   1,385,000,000   |\n"sv,
    "| 2.  India             |   1,339,000,000   |\n"sv,
    "| 3.  United States     |     324,000,000   |\n"sv,
    "| 4.  Indonesia         |     261,000,000   |\n"sv,
    "| 5.  Brazil            |     208,000,000   |\n"sv,
    "| 6.  Pakistan          |     202,000,000   |\n"sv,
    "| 7.  Nigeria           |     193,000,000   |\n"sv,
    "| 8.  Bangladesh        |     166,000,000   |\n"sv,
    "| 9.  Russia            |     144,000,000   |\n"sv,
    "| 10. Mexico            |     129,000,000   |\n"sv,
    "| 11. Japan             |     127,000,000   |\n"sv,
    "| 12. Philippines       |     103,000,000   |\n"sv,
    "| 13. Vietnam           |      93,000,000   |\n"sv,
    "| 14. Ethiopia          |      92,000,000   |\n"sv,
    "| 15. Egypt             |      90,000,000   |\n"sv,
    "| 16. Turkey            |      80,000,000   |\n"sv,
    "| 17. Iran              |      80,000,000   |\n"sv,
    "| 18. Congo (Kinshasa)  |      77,000,000   |\n"sv,
    "| 19. Thailand          |      68,000,000   |\n"sv,
    "| 20. United Kingdom    |      65,110,000   |\n"sv,
    "| 21. France            |      64,000,000   |\n"sv,
    "| 22. Italy             |      63,000,000   |\n"sv,
    "| 23. Germany           |      61,000,000   |\n"sv,
    "| 24. South Korea       |      60,000,000   |\n"sv,
    "+-----------------------+-------------------+\n"sv
  });


  asm("hlt");
  //global_finalize();
}
