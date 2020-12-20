#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "encoder.h"
#include "decoder.h"

int main (int argc, char *argv[])
{
  if (argc != 3)
    {
      printf ("Usage: %s <r> <m>\n", argv[0]);
      return 1;
    }
  int r = atoi (argv[1]);
  int m = atoi (argv[2]);
  printf ("r = %d\n", r);
  printf ("m = %d\n", m);
  if (r < 0 || m < 0 || r >= m)
    {
      printf ("Parameters must satisfy the following relation: 0 < r < m \n");
      return 1;
    }

  double time = clock ();
  RM_encoder encoder (r, m);
  time = (clock() - time) / CLOCKS_PER_SEC;
  std::cout << " Matrix creation time: " << time << std::endl;

  size_t k = encoder.get_k();
  bit_array word = get_rand_bits (k);
  std::cout << "Word to encode:" << std::endl;
  print_bits (word);

  time = clock ();
  bit_array code = encoder.encode(word);
  time = (clock() - time) / CLOCKS_PER_SEC;
  std::cout << " Encoding time: " << time << std::endl;
  std::cout << "Code:" << std::endl;
  print_bits (code);

  size_t noise = max_noise(r, m);
  std::cout << "Max noise:" << noise << std::endl;
  bit_array noise_code = bit_array(code);
  set_noise(noise_code, noise);
  std::cout << "Noise code:" << std::endl;
  print_bits (noise_code);

  RM_decoder decoder (r, m, k);

  time = clock ();
  bit_array decoded_word = decoder.decode (code);
  time = (clock() - time) / CLOCKS_PER_SEC;
  std::cout << " Decoding time: " << time << std::endl;
  std::cout << "Decoded word:" << std::endl;
  print_bits (decoded_word);

  time = clock ();
  bit_array decoded_noise_word = decoder.decode (noise_code);
  time = (clock() - time) / CLOCKS_PER_SEC;
  std::cout << " Decoding with noise time: " << time << std::endl;
  std::cout << "Decoded noise word:" << std::endl;
  print_bits (decoded_noise_word);

  std::cout << "Diff: " << diff (decoded_word, word) << std::endl;

  std::cout << "Diff noise: " << diff (decoded_noise_word, word) << std::endl;

  return 0;
}
