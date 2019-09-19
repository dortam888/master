all: debug release

debug: vsa_debug.out

release: vsa_release.out

review: vsa_review_debug.out vsa_review_release.out

after_review: dor_vsa.out

test: daniel_test.out

vsa_debug.out: vsa.c test/test_vsa_dor.c include/vsa.h
	gcc -ansi -pedantic-errors -Wall -Wextra -g vsa.c test/test_vsa_dor.c -I include -o vsa_debug.out

vsa_release.out: vsa.c test/test_vsa_release_dor.c include/vsa.h
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 vsa.c test/test_vsa_release_dor.c -I include -o vsa_release.out

vsa_review_debug.out: ~/Downloads/vsa_review.c test/test_vsa_dor.c include/vsa.h
	gcc -ansi -pedantic-errors -Wall -Wextra -g ~/Downloads/vsa_review.c test/test_vsa_dor.c -I include -o vsa_review_debug.out

vsa_review_release.out: ~/Downloads/vsa_review.c test/test_vsa_release_dor.c include/vsa.h
	gcc -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -O3 vsa.c test/test_vsa_release_dor.c -I include -o vsa_review_release.out

daniel_test.out: vsa.c ~/Downloads/daniel_test_vsa.c include/vsa.h
	gcc -ansi -pedantic-errors -Wall -Wextra -g vsa.c ~/Downloads/daniel_test_vsa.c -I include -o daniel_test.out

dor_vsa.out: ~/Downloads/dor_vsa.c ~/Downloads/daniel_test_vsa.c include/vsa.h
	gcc -ansi -pedantic-errors -Wall -Wextra -g ~/Downloads/dor_vsa.c ~/Downloads/daniel_test_vsa.c -I include -o dor_vsa_debug.out
