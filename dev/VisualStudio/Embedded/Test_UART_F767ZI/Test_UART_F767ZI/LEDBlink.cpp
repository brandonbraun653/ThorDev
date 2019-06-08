
#include <Chimera/chimera.hpp>
#include <Chimera/threading.hpp>

int main(void)
{
  ChimeraInit();

  Chimera::Threading::startScheduler();

  while (1)
  {
  }
}
