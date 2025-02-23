#include "headers/sv_randomizer_headers/sv_shared_class.h"

json cleanSceneJSON;

SVShared::SVShared(){
    pokemonMapping = readJsonQFile("SV_FLATBUFFERS/pokemon_mapping.json");
    pokemonPersonalData = readJsonQFile("SV_FLATBUFFERS/SV_PERSONAL/personal_array_clean.json");
    maxGeneration = 9;
    maxAllowedId = 1025;
    regionalStage1 = {1, 4, 7, 10, 13, 16, 19, 21, 23, 27, 29, 32, 37, 41, 43, 46, 48, 50, 52, 54, 56, 58, 60, 63, 66, 69, 72,
                      74, 77, 79, 81, 83, 84, 86, 88, 90, 92, 95, 96, 98, 100, 102, 104, 108, 109, 111, 114, 116, 118, 120, 123,
                      129, 133, 137, 138, 140, 147, 152, 155, 158, 161, 163, 165, 167, 170, 172, 173, 174, 175, 177, 179, 187,
                      190, 191, 193, 194, 198, 200, 203, 204, 206, 207, 209, 211, 215, 216, 218, 220, 222, 223, 228, 231, 234,
                      236, 238, 239, 240, 246, 252, 255, 258, 261, 263, 265, 270, 273, 276, 278, 280, 283, 285, 287, 290,
                      293, 296, 298, 299, 300, 304, 307, 309, 316, 318, 320, 322, 325, 328, 331, 333, 339, 341, 343, 345, 347,
                      349, 353, 355, 360, 361, 363, 366, 371, 374, 387, 390, 393, 396, 399, 401, 403, 406, 408, 410, 412, 415,
                      418, 420, 422, 425, 427, 431, 433, 434, 436, 438, 439, 440, 443, 446, 447, 449, 451, 453, 456, 458, 459,
                      495, 498, 501, 504, 506, 509, 511, 513, 515, 517, 519, 522, 524, 527, 529, 532, 535, 540, 543, 546, 548,
                      550, 551, 554, 557, 559, 562, 564, 566, 568, 570, 572, 574, 577, 580, 582, 585, 588, 590, 592, 595, 597,
                      599, 602, 605, 607, 610, 613, 616, 619, 622, 624, 627, 629, 633, 636, 650, 653, 656, 659, 661, 664, 667,
                      669, 672, 674, 677, 679, 682, 684, 686, 688, 690, 692, 694, 696, 698, 704, 708, 710, 712, 714, 722, 725,
                      728, 731, 734, 736, 739, 742, 744, 747, 749, 751, 753, 755, 757, 759, 761, 767, 769, 772, 782, 789, 803,
                      810, 813, 816, 819, 821, 824, 827, 829, 831, 833, 835, 837, 840, 843, 846, 848, 850, 852, 854, 856,
                      859, 868, 872, 878, 884, 885, 906, 909, 912, 915, 917, 919, 921, 924, 926, 928, 932, 935, 938, 940,
                      942, 944, 946, 948, 951, 953, 955, 957, 960, 963, 965, 969, 971, 974, 996, 999, 1012};
    regionalStage2 = {2, 5, 8, 11, 14, 17, 25, 30, 33, 35, 39, 42, 44, 57, 61, 64, 67, 70, 75, 82, 93, 112, 113, 117, 122, 125,
                      126, 148, 153, 156, 159, 176, 180, 183, 188, 217, 221, 233, 247, 253, 256, 259, 266, 268, 271, 274, 281,
                      288, 294, 305, 315, 329, 356, 364, 372, 375, 388, 391, 394, 397, 404, 444, 496, 499, 502, 507, 520, 525,
                      533, 536, 541, 544, 552, 575, 578, 583, 600, 603, 608, 611, 625, 634, 651, 654, 657, 662, 665, 670, 680,
                      705, 723, 726, 729, 732, 737, 762, 783, 811, 814, 817, 822, 825, 838, 857, 860, 886, 907, 910, 913,
                      922, 929, 933, 958, 997, 1011};
    regionalStage3 = {3, 6, 9, 12, 15, 18, 20, 22, 24, 26, 28, 31, 34, 36, 38, 40, 45, 47, 49, 51, 53, 55, 59, 62, 65, 68, 71,
                      73, 76, 78, 80, 85, 87, 89, 91, 94, 97, 99, 101, 103, 105, 106, 107, 110, 119, 121, 124, 130, 134, 135,
                      136, 139, 141, 143, 149, 154, 157, 160, 162, 164, 166, 168, 169, 171, 178, 181,
                      182, 184, 185, 186, 189, 192, 195, 196, 197, 199, 202, 205, 208, 210, 212, 219, 224, 226, 229, 230, 232,
                      237, 242, 248, 254, 257, 260, 262, 264, 267, 269, 272, 275, 277, 279, 282,
                      284, 286, 289, 291, 292, 295, 297, 301, 306, 308, 310, 317, 319, 321, 323, 326, 330, 332, 334, 340, 342,
                      344, 346, 348, 350, 354, 358, 362, 365, 367, 368, 373, 376, 389, 392, 395, 398, 400, 402, 405, 407, 409,
                      411, 413, 414, 416, 419, 421, 423, 424, 426, 428, 429,
                      430, 432, 435, 437, 445, 448, 450, 452, 454, 457, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470,
                      471, 472, 473, 474, 475, 476, 477, 478, 479, 497, 500, 503, 505, 508, 510, 512, 514, 516, 518, 521,
                      523, 526, 528, 530, 534, 537, 542,
                      545, 547, 549, 553, 555, 558, 560, 563, 565, 567, 569, 571, 573, 576, 579, 581, 584, 586, 589, 591, 593,
                      596, 598, 601, 604, 606, 609, 612, 614, 617, 620, 623, 628, 630, 635, 637,
                      652, 655, 658, 660, 663, 666, 668, 671, 673, 675, 678, 681, 683, 685, 687,
                      689, 691, 693, 695, 697, 699, 700, 706, 709, 711, 713, 715, 724, 727, 730,
                      733, 735, 738, 740, 743, 745, 748, 750, 752, 754, 756, 758, 760, 763, 768, 770, 773, 784,
                      804, 805, 806, 812, 815, 818,
                      820, 823, 826, 828, 830, 832, 834, 836, 839, 841, 842, 844, 847, 849, 851, 853, 855, 858, 861, 862, 863,
                      864, 865, 866, 867, 869, 873, 879, 887, 899, 900, 901,
                      902, 903, 904, 908, 911, 914, 916, 918, 920, 923, 925, 927, 930, 934, 936, 937, 939, 941, 943, 945,
                      947, 949, 952, 954, 956, 961, 964, 966, 970, 972, 975, 979, 980, 981, 982, 983, 984, 985, 986, 987,
                      988, 989, 990, 991, 992, 993, 994, 995, 998, 1000, 1005, 1006,
                      1013, 1018, 1019};
    regionalSingleStage = {115, 127, 128, 131, 132, 142, 201, 213, 214, 225, 227, 235, 241, 302, 303, 311, 312, 313, 314, 324, 327,
                           335, 336, 337, 338, 351, 352, 357, 359, 369, 370, 417, 441, 442, 455, 531, 538, 539, 556, 561, 587, 594,
                           615, 618, 621, 626, 631, 632, 676, 701, 702, 703, 707, 741, 746, 764, 765, 766, 771, 774, 775, 776, 777,
                           778, 779, 780, 781, 845, 870, 871, 874, 875, 876, 877, 880, 881, 882, 883, 931, 950, 959, 962, 967, 968, 973,
                           976, 977, 978};
    allowedMoves = {1, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30, 31, 32, 33,
                    34, 35, 36, 37, 38, 39, 40, 42, 43, 44, 45, 46, 47, 48, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
                    62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 88, 89,
                    90, 91, 92, 93, 94, 95, 97, 98, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 113, 114,
                    115, 116, 118, 120, 122, 123, 124, 126, 127, 129, 133, 135, 136, 137, 138, 139, 141, 143, 144, 147,
                    150, 151, 152, 153, 154, 156, 157, 160, 161, 162, 163, 164, 166, 167, 168, 172, 173, 174, 175,
                    176, 177, 178, 179, 180, 181, 182, 183, 184, 186, 187, 188, 189, 191, 192, 194, 195, 196, 197, 198,
                    199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 217, 219, 220,
                    221, 223, 224, 225, 226, 227, 229, 230, 231, 232, 234, 235, 236, 238, 239, 240, 241, 242, 243, 244,
                    245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 259, 260, 261, 262, 263, 264, 266,
                    268, 269, 270, 271, 272, 273, 275, 276, 278, 280, 281, 282, 283, 284, 285, 286, 291, 292, 294, 295,
                    296, 297, 298, 299, 303, 304, 305, 306, 307, 308, 309, 310, 311, 313, 314, 315, 317, 319, 321, 322,
                    323, 325, 326, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344,
                    345, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 359, 360, 361, 362, 364, 365, 366, 367, 368,
                    369, 370, 371, 372, 374, 379, 380, 383, 384, 385, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396,
                    397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416,
                    417, 418, 419, 420, 421, 422, 423, 424, 425, 427, 428, 430, 432, 433, 434, 435, 436, 437, 438, 439,
                    440, 441, 442, 444, 446, 447, 449, 450, 451, 452, 453, 454, 455, 457, 458, 459, 460, 461, 463, 467,
                    468, 469, 470, 471, 472, 473, 474, 476, 478, 479, 482, 483, 484, 486, 487, 488, 489, 490, 491, 492,
                    493, 494, 495, 496, 497, 499, 500, 501, 502, 503, 504, 505, 506, 508, 509, 510, 511, 512, 513, 514,
                    515, 517, 518, 519, 520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 532, 533, 534, 535, 538, 539,
                    540, 541, 542, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 562, 564, 565,
                    566, 568, 570, 571, 572, 573, 574, 575, 576, 577, 580, 581, 583, 584, 585, 586, 587, 589, 590, 591,
                    592, 593, 594, 595, 596, 597, 598, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 618, 619, 620,
                    621, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 672, 675, 676, 678, 679, 680, 681,
                    682, 683, 684, 686, 688, 689, 690, 691, 692, 693, 694, 705, 706, 707, 709, 710, 711, 713, 714, 715,
                    716, 722, 744, 745, 746, 747, 748, 749, 750, 751, 752, 756, 775, 776, 777, 778, 780, 781, 782, 783,
                    784, 785, 786, 787, 788, 789, 790, 791, 793, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806,
                    807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826,
                    827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846,
                    847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 866,
                    867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 884, 885, 886,
                    887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899, 900, 901, 902, 903, 904, 905, 906,
                    907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919};
    bannedPokemon = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 29, 30, 31, 32, 33, 34, 41, 42, 46, 47, 63,
                     64, 65, 66, 67, 68, 77, 78, 83, 95, 98, 99, 104, 105, 108, 114, 115, 118, 119, 120, 121, 122, 124, 127,
                     138, 139, 140, 141, 142, 165, 166, 169, 175, 176, 177, 178, 201, 202, 208, 213, 222, 223, 224, 226,
                     238, 241, 251, 263, 264, 265, 266, 267, 268, 269, 276, 277, 290, 291, 292, 293, 294, 295, 300, 301,
                     303, 304, 305, 306, 309, 310, 315, 318, 319, 320, 321, 327, 337, 338, 343, 344, 345, 346, 347, 348,
                     351, 352, 359, 360, 363, 364, 365, 366, 367, 368, 369, 399, 400, 406, 407, 412, 413, 414, 420, 421,
                     427, 428, 431, 432, 439, 441, 451, 452, 455, 458, 463, 465, 468, 494, 504, 505, 506, 507, 508, 509,
                     510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 524, 525, 526, 527, 528, 531, 535, 536,
                     537, 538, 539, 543, 544, 545, 554, 555, 556, 557, 558, 561, 562, 563, 564, 565, 566, 567, 568, 569,
                     582, 583, 584, 587, 588, 589, 592, 593, 597, 598, 599, 600, 601, 605, 606, 616, 617, 618, 621, 626,
                     631, 632, 649, 659, 660, 674, 675, 676, 679, 680, 681, 682, 683, 684, 685, 688, 689, 694, 695, 696,
                     697, 698, 699, 710, 711, 716, 717, 718, 746, 755, 756, 759, 760, 767, 768, 771, 772, 773, 776, 777,
                     780, 781, 785, 786, 787, 788, 793, 794, 795, 796, 797, 798, 799, 802, 803, 804, 805, 806, 807, 808,
                     809, 824, 825, 826, 827, 828, 829, 830, 831, 832, 835, 836, 850, 851, 852, 853, 864, 865, 866, 867,
                     880, 881, 882, 883, 862};
    bannedItems = {
        2311, 2313, 2314, 2315, 2316, 2317, 2318, 2319, 2320, 2321, 2322, 2323, 2324, 2325, 2326, 2327, 2329,
        2330, 2331, 2332, 2333, 2334, 2335, 2336, 2337, 2338, 2339, 2340, 2341, 2342, 2348, 2349, 2350, 2351,
        2352, 2353, 2354, 2355, 2356, 2357, 2358, 2359, 2360, 2361, 2362, 2363, 2364, 2365, 2366, 2367, 2368,
        2369, 2370, 2371, 2372, 2373, 2374, 2375, 2376, 2377, 2378, 2379, 2380, 2381, 2382, 2383, 2384, 2385,
        2386, 2387, 2388, 2389, 2390, 2391, 2392, 2393, 2394, 2395, 2396, 1857, 1858, 1889, 1890, 1891, 1892,
        1893, 1894, 1895, 1896, 1897, 1898, 1899, 1900, 1901, 1902, 1903, 1909, 1910, 1911, 1912, 1913, 1914,
        1915, 1916, 1917, 1918, 1919, 1920, 1921, 1922, 1923, 1924, 1925, 1926, 1927, 1928, 1929, 1930, 1931,
        1932, 1933, 1934, 1935, 1936, 1937, 1938, 1939, 1940, 1941, 1942, 1943, 1944, 1945, 1946, 2417, 2418,
        2419, 2420, 2421, 2422, 2423, 2424, 2425, 2426, 2427, 2428, 2429, 2430, 2431, 2432, 2433, 2434, 2435,
        2436, 2437, 2551, 2552};
    pokeballNames = {"Poke Ball", "Great Ball", "Ultra Ball", "Master Ball", "Beast Ball",
                     "Cherish Ball", "Luxury Ball", "Timer Ball", "Net Ball", "Nest Ball", "Dive Ball", "Dusk Ball",
                     "Repeat Ball", "Premier Ball", "Heal Ball", "Quick Ball", "Fast Ball", "Level Ball", "Lure Ball",
                     "Heavy Ball","Love Ball","Friend Ball","Moon Ball","Sport Ball","Safari Ball","Dream Ball"};
}

std::string SVShared::getPokemonItemId(int index, int form){
    if (paradox.contains(index) && index != 1007 && index != 1008 && index != 1024) {
        return "ITEMID_BUUSUTOENAJII";
    }

    switch (index) {
        case 25: return "ITEMID_DENKIDAMA";
        case 113:
        case 242: return "ITEMID_MANMARUISI";
        case 283:
        case 415: return "ITEMID_AMAIMITU";
        case 285:
        case 590:
        case 948: return "ITEMID_TIISANAKINOKO";
        case 286:
        case 591:
        case 949: return "ITEMID_TIISANAKINOKO";
        case 316: return "ITEMID_ORENNOMI";
        case 317: return "ITEMID_OBONNOMI";
        case 440: return "ITEMID_MANMARUISI";
        case 625: return "ITEMID_KASIRANOAKASI";
        case 734: return "ITEMID_MOMONNOMI";
        case 739: return "ITEMID_NANASINOMI";
        case 740: return "ITEMID_KURABONOMI";
        case 741: return "ITEMID_YAMABUKINOMITU";
        case 778: return "ITEMID_KAGONOMI";
        case 819: return "ITEMID_ORENNOMI";
        case 483:
            if (form == 1) return "ITEMID_DAIKONGOUDAMA";
            break;
        case 484:
            if (form == 1) return "ITEMID_DAISIRATAMA";
            break;
        case 487:
            if (form == 1) return "ITEMID_DAIHAKKINDAMA";
            break;
        case 493:
            switch (form) {
            case 1: return "ITEMID_KOBUSINOPUREETO";
            case 2: return "ITEMID_AOZORAPUREETO";
            case 3: return "ITEMID_MOUDOKUPUREETO";
            case 4: return "ITEMID_DAITINOPUREETO";
            case 5: return "ITEMID_GANSEKIPUREETO";
            case 6: return "ITEMID_TAMAMUSIPUREETO";
            case 7: return "ITEMID_MONONOKEPUREETO";
            case 8: return "ITEMID_KOUTETUPUREETO";
            case 9: return "ITEMID_HINOTAMAPUREETO";
            case 10: return "ITEMID_SIZUKUPUREETO";
            case 11: return "ITEMID_MIDORINOPUREETO";
            case 12: return "ITEMID_IKAZUTIPUREETO";
            case 13: return "ITEMID_HUSIGINOPUREETO";
            case 14: return "ITEMID_TURARANOPUREETO";
            case 15: return "ITEMID_RYUUNOPUREETO";
            case 16: return "ITEMID_KOWAMOTEPUREETO";
            case 17: return "ITEMID_SEIREIPUREETO";
            }
            break;
        case 888:
            if (form == 1) return "ITEMID_KUTITATURUGI";
            break;
        case 889:
            if (form == 1) return "ITEMID_KUTITATATE";
            break;
        case 1017:
            switch (form) {
                case 1: return "ITEMID_IDONOMEN";
                case 2: return "ITEMID_KAMADONOMEN";
                case 3: return "ITEMID_ISHIDUENOMEN";
            }
        break;
    }
    return "ITEMID_NONE";
}

int SVShared::getPokemonItemValue(int index, int form){
    if (paradox.contains(index) && index != 1007 && index != 1008 && index != 1024) {
        return 25;
    }

    switch (index) {
    case 25: return 5;
    case 113:
    case 242: return 30;
    case 283:
    case 415: return 5;
    case 285:
    case 590:
    case 948: return 5;
    case 286:
    case 591:
    case 949: return 30;
    case 316: return 30;
    case 317: return 5;
    case 440: return 5;
    case 625: return 100;
    case 734: return 5;
    case 739: return 5;
    case 740: return 5;
    case 741: return 5;
    case 778: return 5;
    case 819: return 5;
    case 483:
        if (form == 1) return 100;
        break;
    case 484:
        if (form == 1) return 100;
        break;
    case 487:
        if (form == 1) return 100;
        break;
    case 493:
        switch (form) {
        case 1: case 2: case 3: case 4: case 5:
        case 6: case 7: case 8: case 9: case 10:
        case 11: case 12: case 13: case 14: case 15:
        case 16: case 17: return 100;
        }
        break;
    case 888:
        if (form == 1) return 100;
        break;
    case 889:
        if (form == 1) return 100;
        break;
    case 1017:
        switch (form) {
        case 1: case 2: case 3: return 100;
        }
        break;
    }
    return 0;
}

std::string SVShared::getItemForPokemon(int pokemon, int form){
    switch(pokemon){
        case 483:
            if(form == 1){
                return "ITEMID_DAIKONGOUDAMA";
            }
            break;
        case 484:
            if(form == 1){
                return "ITEMID_DAISIRATAMA";
            }
            break;
        case 487:
            if(form == 1){
                return "ITEMID_DAIHAKKINDAMA";
            }
            break;
        case 493:
            switch(form){
                case 1:  // Fightning
                    return "ITEMID_KOBUSINOPUREETO";
                    break;
                case 2:  // Flying
                    return "ITEMID_AOZORAPUREETO";
                    break;
                case 3:  // poison
                    return "ITEMID_MOUDOKUPUREETO";
                    break;
                case 4:  // ground
                    return "ITEMID_DAITINOPUREETO";
                    break;
                case 5:  // rock
                    return "ITEMID_GANSEKIPUREETO";
                    break;
                case 6:  // bug
                    return "ITEMID_TAMAMUSIPUREETO";
                    break;
                case 7:  // ghost
                    return "ITEMID_MONONOKEPUREETO";
                    break;
                case 8:  // steel
                    return "ITEMID_KOUTETUPUREETO";
                    break;
                case 9:  // fire
                    return "ITEMID_HINOTAMAPUREETO";
                    break;
                case 10:  // water
                    return "ITEMID_SIZUKUPUREETO";
                    break;
                case 11:  // grass
                    return "ITEMID_MIDORINOPUREETO";
                    break;
                case 12:  // electric
                    return "ITEMID_IKAZUTIPUREETO";
                    break;
                case 13:  // psychic
                    return "ITEMID_HUSIGINOPUREETO";
                    break;
                case 14:  // ice
                    return "ITEMID_TURARANOPUREETO";
                    break;
                case 15:  // dragon
                    return "ITEMID_RYUUNOPUREETO";
                    break;
                case 16:  // dark
                    return "ITEMID_KOWAMOTEPUREETO";
                    break;
                case 17:  // Fairy
                    return "ITEMID_SEIREIPUREETO";
                    break;
            };
            break;
        case 888:
            if(form == 1){
                return "ITEMID_KUTITATURUGI";
            }
            break;
        case 889:
            if(form == 1){
                return "ITEMID_KUTITATATE";
            }
            break;
        case 1017:
            switch(form){
                case 1:
                    return "ITEMID_IDONOMEN";
                    break;
                case 2:
                    return "ITEMID_KAMADONOMEN";
                    break;
                case 3:
                    return "ITEMID_ISHIDUENOMEN";
                    break;
            };
            break;
        default:
            return "ITEMID_NONE";
    };

    return "ITEMID_NONE";
}

std::string SVShared::selectTeraTypes(int pokemon, int formId){

    switch(pokemon){
        case 1017:
            switch(formId){
                case 0:
                    return "KUSA";
                case 1:
                    return "MIZU";
                case 2:
                    return "HONOO";
                case 3:
                    return "IWA";
            }
                break;
        case 1024:
            return "NIJI";
        default:
            break;
    }

    return teraTypes[std::rand()%teraTypes.length()].toUpper().toStdString();
}

bool SVShared::CheckAllowedMons(QList<int>& allowedPokemon){
    int i = 0;
    while ((i+1) < allowedPokemon.size())
    {
        int checka = allowedPokemon[i];
        int ii = i+1;
        while (ii < allowedPokemon.size())
        {
            int checkb = allowedPokemon[ii];
            if (checkb == checka)
            {
            qDebug() << "Allow check, double up ID: " << checkb;
            allowedPokemon.removeAt(ii);
            }else{
            ii++;
            }
        }
        i++;
    }
    return true;
}

int SVShared::GenerateAllowedMons(allowedPokemonLimiter Limiter, QList<int>& allowedPokemon){
    // Logic for Poke Limiter
    QList<int> RemoveList;
    QList<int> AllowList;
    for(int i=0; i<9; i++){
        if(Limiter.Gens[i] == true){
            switch (i){
            case 0:
                AllowList += pokemonDevIds_Gen1;
                break;
            case 1:
                AllowList += pokemonDevIds_Gen2;
                break;
            case 2:
                AllowList += pokemonDevIds_Gen3;
                break;
            case 3:
                AllowList += pokemonDevIds_Gen4;
                break;
            case 4:
                AllowList += pokemonDevIds_Gen5;
                break;
            case 5:
                AllowList += pokemonDevIds_Gen6;
                break;
            case 6:
                AllowList += pokemonDevIds_Gen7;
                break;
            case 7:
                AllowList += pokemonDevIds_Gen8;
                break;
            case 8:
                AllowList += pokemonDevIds_Gen9;
                break;
            };
        }
        switch (i){
        case 0:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen1_legends;
            }else{
                AllowList += gen1_legends;
            }
            break;
        case 1:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen2_legends;
            }else{
                AllowList += gen2_legends;
            }
            break;
        case 2:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen3_legends;
            }else{
                AllowList += gen3_legends;
            }
            break;
        case 3:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen4_legends;
            }else{
                AllowList += gen4_legends;
            }
            break;
        case 4:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen5_legends;
            }else{
                AllowList += gen5_legends;
            }
            break;
        case 5:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen6_legends;
            }else{
                AllowList += gen6_legends;
            }
            break;
        case 6:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen7_legends;
            }else{
                AllowList += gen7_legends;
            }
            break;
        case 7:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen8_legends;
            }else{
                AllowList += gen8_legends;
            }
            break;
        case 8:
            if(Limiter.GenLegends[i] == false){
                RemoveList += gen9_legends;
            }else{
                AllowList += gen9_legends;
            }
            break;
        };
    }

    if(Limiter.Paradox == false){
        RemoveList += paradox;
    }else{
        AllowList += paradox;
    }
    if(Limiter.UltraBeast == false){
        RemoveList += UB;
    }else{
        AllowList += UB;
    }
    QSet<int> AllowSet(AllowList.begin(), AllowList.end());
    QSet<int> RemoveSet(RemoveList.begin(), RemoveList.end());
    AllowSet.subtract(RemoveSet);
	
    if(Limiter.Stage1 == false){
        QSet<int> stage1Set(gen9Stage1.begin(), gen9Stage1.end());
        AllowSet.subtract(stage1Set);
    }
    if(Limiter.Stage2 == false){
        QSet<int> stage2Set(gen9Stage2.begin(), gen9Stage2.end());
        AllowSet.subtract(stage2Set);
    }
    if(Limiter.Stage3 == false){
        QSet<int> stage3Set(gen9Stage3.begin(), gen9Stage3.end());
        AllowSet.subtract(stage3Set);
    }
    if(Limiter.SingleStage == false){
        QSet<int> singleStageSet(no_evolution.begin(), no_evolution.end());
        AllowSet.subtract(singleStageSet);
    }
	
    QSet<int> bannedSet(banned_pokemon.begin(), banned_pokemon.end());
    AllowSet.subtract(bannedSet);

    allowedPokemon = QList<int>(AllowSet.begin(), AllowSet.end());
    //organise list
    std::sort(allowedPokemon.begin(), allowedPokemon.end());
    //qDebug() << allowedPokemon.size();
    CheckAllowedMons(allowedPokemon);
    return allowedPokemon.size();
}

void SVShared::getUsablePokemon(QVector<bool> generations, bool legend, bool paradoxs, bool legends_paradox, QList<int>& allowedPokemon, QList<int>& allowedLegends){
    // Logic for generation limiter
    int totalGens = 0;
    for(int i=0; i<9; i++){
        if(generations[i] == true){
            // Logic
            switch (i){
            case 0:
                if(legend == true){
                    allowedPokemon += gen1_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen1;
                    allowedLegends += gen1_legends;
                }
                break;
            case 1:
                if(legend == true){
                    allowedPokemon += gen2_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen2;
                    allowedLegends += gen2_legends;
                }
                break;
            case 2:
                if(legend == true){
                    allowedPokemon += gen3_legends;
                }
                else{
                    allowedPokemon += pokemonDevIds_Gen3;
                    allowedLegends += gen3_legends;
                }
                break;
            case 3:
                if(legend == true){
                    allowedPokemon += gen4_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen4;
                    allowedLegends += gen4_legends;
                }
                break;
            case 4:
                if(legend == true){
                    allowedPokemon += gen5_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen5;
                    allowedLegends += gen5_legends;
                }
                break;
            case 5:
                if(legend == true){
                    allowedPokemon += gen6_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen6;
                    allowedLegends += gen6_legends;
                }
                break;
            case 6:
                if(legend == true){
                    allowedPokemon += gen7_legends;
                    allowedPokemon += UB;
                }
                else{
                    allowedPokemon += pokemonDevIds_Gen7;
                    allowedLegends += gen7_legends;
                    allowedLegends += UB;
                }
                break;
            case 7:
                if(legend == true){
                    allowedPokemon += gen8_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen8;
                    allowedLegends += gen8_legends;
                }
                break;
            case 8:
                if(legends_paradox == true){
                    allowedPokemon += gen9_legends;
                    allowedPokemon += paradox;
                }else if(legend == true){
                    allowedPokemon += gen9_legends;
                }else if(paradoxs == true){
                    allowedPokemon += paradox;
                }
                else{
                    allowedPokemon += pokemonDevIds_Gen9;
                    allowedLegends += gen9_legends;
                    allowedLegends += paradox;
                }
                break;
            };
            totalGens++;
        }
    }
    if(totalGens == 0){
        allowedPokemon = {};
        allowedPokemon += pokemonDevIds_Gen1;
        allowedLegends += gen1_legends;
        allowedPokemon += pokemonDevIds_Gen2;
        allowedLegends += gen2_legends;
        allowedPokemon += pokemonDevIds_Gen3;
        allowedLegends += gen3_legends;
        allowedPokemon += pokemonDevIds_Gen4;
        allowedLegends += gen4_legends;
        allowedPokemon += pokemonDevIds_Gen5;
        allowedLegends += gen5_legends;
        allowedPokemon += pokemonDevIds_Gen6;
        allowedLegends += gen6_legends;
        allowedPokemon += pokemonDevIds_Gen7;
        allowedLegends += gen7_legends;
        allowedLegends += UB;
        allowedPokemon += pokemonDevIds_Gen8;
        allowedLegends += gen8_legends;
        allowedPokemon += pokemonDevIds_Gen9;
        allowedLegends += gen9_legends;
        allowedLegends += paradox;
    }

    // Legends and Paradox > Legends > Paradox
    if(legends_paradox == true){
        allowedPokemon = {};
        allowedPokemon = legends_and_paradox;
    }else if(legend == true){
        allowedPokemon = {};
        allowedPokemon = legends;
    }else if(paradoxs == true){
        allowedPokemon = {};
        allowedPokemon = paradox;
    }
}

void SVShared::getBannedPokemon(bool stage1, bool stage2, bool stage3, bool singlestage, QList<int>& allowedPokemon){
    QSet<int> allowedMons(allowedPokemon.begin(), allowedPokemon.end());

    if(stage1 == true){
        QSet<int> stage1Set(gen9Stage1.begin(), gen9Stage1.end());
        allowedMons.subtract(stage1Set);
    }

    if(stage2 == true){
        QSet<int> stage2Set(gen9Stage2.begin(), gen9Stage2.end());
        allowedMons.subtract(stage2Set);
    }

    if(stage3 == true){
        QSet<int> stage3Set(gen9Stage3.begin(), gen9Stage3.end());
        allowedMons.subtract(stage3Set);
    }

    if(singlestage == true){
        QSet<int> singleStageSet(no_evolution.begin(), no_evolution.end());
        allowedMons.subtract(singleStageSet);
    }

    QSet<int> bannedSet(banned_pokemon.begin(), banned_pokemon.end());
    allowedMons.subtract(bannedSet);

    allowedPokemon = QList<int>(allowedMons.begin(), allowedMons.end());
}

void SVShared::createFolderHierarchy(const std::string& folder) {
    try {
        // Convert to absolute path
        fs::path absolutePath = fs::absolute(folder);
        //qDebug() << "Absolute path: " << QString::fromUtf8(absolutePath.string());

        // Create directories
        fs::create_directories(absolutePath);
        //qDebug() << "Created path: " << QString::fromUtf8(absolutePath.string());

        // Define where to start (example folder)
        std::string startFolder = "output";
        bool startSettingPermissions = false;
        fs::path currentPath;

        for (auto it = absolutePath.begin(); it != absolutePath.end(); ++it) {
            if (*it == startFolder) {
                startSettingPermissions = true;
            }

            if (startSettingPermissions) {
                currentPath /= *it; // Incrementally build the path
                if (!fs::exists(currentPath)) {
                    fs::create_directory(currentPath); // Create directory if it doesn't exist
                }
                fs::permissions(currentPath, fs::perms::all, fs::perm_options::add);
                qDebug() << "Set permissions for: " << QString::fromUtf8(currentPath.string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        qCritical() << "Error creating folder hierarchy: " << e.what();
    }
}

int SVShared::generateBinary(const std::string& schema, const std::string& jsonFile, const std::string& path, bool debug) {
    // Determine the platform and architecture
    std::string platform;
    std::string architecture;

    #ifdef _WIN32
        platform = "Windows";
    #ifdef _WIN64
        architecture = "Win64";
    #else
        architecture = "Win32";
    #endif
    #elif __APPLE__
        platform = "macOS";
    #ifdef __aarch64__
        architecture = "ARM";
    #else
        architecture = "Intel";
    #endif
    #elif __linux__
        platform = "Linux";
    #ifdef __clang__
        architecture = "Clang";
    #else
        architecture = "GCC";
    #endif
    #else
        platform = "Unknown";
        architecture = "Unknown";
    #endif

    // Print detected platform and architecture if debug is enabled
    qDebug()<< QString("Platform %1 with Architecture %2").arg(QString::fromStdString(platform), QString::fromStdString(architecture));

    // Determine the binary path based on platform and architecture
    std::string flatc;
    if (platform == "Windows") {
        flatc = fs::absolute("thirdparty/flatbuffers/windows/flatc.exe").string();
    } else if (platform == "macOS") {
        flatc = (architecture == "ARM")
        ? fs::absolute("thirdparty/flatbuffers/mac_ant/flatc").string()
        : fs::absolute("thirdparty/flatbuffers/mac_intel/flatc").string();
    } else if (platform == "Linux") {
        flatc = (architecture == "Clang")
        ? fs::absolute("thirdparty/flatbuffers/linux_clang/flatc").string()
        : fs::absolute("thirdparty/flatbuffers/linux_gcc/flatc").string();
    } else {
        throw std::runtime_error("Unsupported platform or architecture");
    }

    // Create the folder hierarchy
    createFolderHierarchy("output/romfs/" + path + "/");
    std::string outpath = fs::absolute("output/romfs/" + path + "/").string();

    // Use QProcess to execute the command
    QProcess process;
    QString program = QString::fromStdString(flatc);

    // Prepare the arguments
    QStringList arguments;
    arguments << "-b"
              << "-o" << QString::fromStdString(outpath)
              << QString::fromStdString(fs::absolute(schema).string())
              << QString::fromStdString(fs::absolute(jsonFile).string())
              << "--no-warnings";

    // Debug the command
    // qDebug()<< QString("Executing: %1 with Arguments: %2").arg(program, arguments.join(' '));

    // Start the process
    process.start(program, arguments);
    process.waitForFinished();

    // Check the process result
    int returnCode = process.exitCode();
    if (debug) {
        if (returnCode != 0) {
            qCritical()<< "Error executing command. Return code: " << returnCode;
            qCritical() << "Error output: " << process.readAllStandardError().toStdString();
        } else {
            qDebug()<< "Program Ended Successfully";
        }
    }

    return returnCode;
}

uint64_t SVShared::fnv1a_64(const std::string& str, uint64_t basis) {
    uint64_t hash = basis;
    for (char c : str) {
        hash ^= static_cast<uint64_t>(c);
        hash *= 0x100000001B3;
    }
    return hash;
}

void SVShared::patchFileDescriptor() {
    using json = nlohmann::json;
    namespace fs = std::filesystem;

    // Load the JSON file
    std::ifstream file(fs::absolute("SV_FLATBUFFERS/SV_DATA_FLATBUFFERS/data_clean.json").string());
    if (!file.is_open()) {
        qFatal()<< "Error: Could not open data_clean.json!";
        return;
    }

    json data;
    file >> data;
    file.close();

    // Define the prefix and modPath
    std::string prefix = fs::current_path().string() + "/output/romfs/";
    fs::path modPath = fs::current_path() / "output/romfs";

    // Iterate through the files in modPath
    for (const auto& item : fs::recursive_directory_iterator(modPath)) {
        if (fs::is_regular_file(item)) {
            std::string name = item.path().string();
            std::replace(name.begin(), name.end(), '\\', '/'); // Replace backslashes with forward slashes
            name = name.substr(prefix.length()); // Remove the prefix

            // Compute the hash
            uint64_t hash = fnv1a_64(name);

            // Check if the hash exists in data["file_hashes"]
            auto& file_hashes = data["file_hashes"];
            auto it = std::find(file_hashes.begin(), file_hashes.end(), hash);
            if (it != file_hashes.end()) {
                size_t index = std::distance(file_hashes.begin(), it);
                file_hashes.erase(it); // Remove hash
                data["files"].erase(data["files"].begin() + index); // Remove corresponding file entry
            }
        }
    }

    // Write the modified data back to a JSON file
    std::ofstream outfile(fs::absolute("SV_FLATBUFFERS/SV_DATA_FLATBUFFERS/data.json").string());
    if (!outfile.is_open()) {
        qFatal()<< "Error: Could not open data.json for writing!";
        return;
    }

    outfile << data.dump(2);
    outfile.close();

    qDebug() << "Patched trpfd!";
}


int modelCount = 0;
int fieldCount = 0;

void SVShared::recursiveFindOfPokemonSceneTable(json& sceneObject, std::vector<int> devId, std::vector<int> formId,std::vector<int> gender, std::vector<bool> rare){
    if (!sceneObject.is_array()) {
        return;
    }
    for(unsigned long long i =0; i<sceneObject.size(); i++){
        if(sceneObject[i]["Type"] == "ti_PokemonModelComponent" || sceneObject[i]["Type"] == "ti_FieldPokemonComponent"){
            qDebug()<<"Type: "<< QString::fromUtf8(sceneObject[i]["Type"].get<std::string>().c_str());

            const auto& dataArray = sceneObject[i]["Data"];
            std::vector<uint8_t> data;
            for (const auto& byteValue : dataArray) {
                if (byteValue.is_number_unsigned()) {
                    data.push_back(static_cast<uint8_t>(byteValue.get<uint32_t>()));
                } else {
                    qFatal() << "Invalid byte value in Data array.\n";
                }
            }

            if(sceneObject[i]["Type"] == "ti_PokemonModelComponent"){
                auto originalTable = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                    data.data());

                qDebug() << "TIPokemonModelComponent:";
                printSceneTables(originalTable);

                flatbuffers::FlatBufferBuilder builder;
                uint16_t dev_id = devId[modelCount];
                uint16_t form_id = formId[modelCount];
                int8_t sex_num = gender[modelCount];
                int8_t rare_bool = rare[modelCount];
                modelCount++;

                // Serialize the new table with updated values
                auto updatedTable = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIPokemonModelComponent(
                    builder,
                    dev_id,
                    form_id,
                    sex_num,
                    rare_bool,
                    originalTable->Field_04(),
                    originalTable->Field_05(),
                    originalTable->Field_06(),
                    originalTable->Field_07(),
                    originalTable->Field_08()
                    );

                // Finish the FlatBuffer
                builder.Finish(updatedTable);

                // Replace the data vector with the new FlatBuffer content
                data.assign(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
                json newTablePokemon = json::array();
                for(uint8_t byte: data){
                    newTablePokemon.push_back(byte);
                }

                sceneObject[i]["Data"] = newTablePokemon;


                auto modifiedTale = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                    data.data());

                qDebug() << "TIPokemonModelComponent (Updated)";
                printSceneTables(modifiedTale);


            }else if(sceneObject[i]["Type"] == "ti_FieldPokemonComponent"){

                auto originalTable = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                    data.data());

                qDebug() << "TIFieldPokemonComponent:\n";
                printSceneTables(originalTable);

                flatbuffers::FlatBufferBuilder builder;
                uint16_t dev_id = devId[fieldCount];
                uint16_t form_id = formId[fieldCount];
                int8_t sex_num = gender[fieldCount];
                int8_t rare_bool = rare[fieldCount];
                fieldCount++;

                // Serialize the new table with updated values
                auto updatedTable = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIFieldPokemonComponent(
                    builder,
                    dev_id,
                    form_id,
                    sex_num,
                    rare_bool,
                    originalTable->Field_04(),
                    originalTable->Field_05(),
                    originalTable->Field_06(),
                    originalTable->Field_07(),
                    originalTable->Field_08()
                    );

                // Finish the FlatBuffer
                builder.Finish(updatedTable);

                // Replace the data vector with the new FlatBuffer content
                data.assign(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
                json newTablePokemon = json::array();
                for(uint8_t byte: data){
                    newTablePokemon.push_back(byte);
                }

                sceneObject[i]["Data"] = newTablePokemon;


                auto modifiedTale = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                    data.data());

                qDebug() << "TIFieldPokemonComponent (Updated):";
                printSceneTables(modifiedTale);
            }
        }
        recursiveFindOfPokemonSceneTable(sceneObject[i]["SubObjects"], devId, formId, gender, rare);
}
}
void SVShared::modifyPokemonScene(std::vector<int> devId, std::vector<int> formId,std::vector<int> gender, std::vector<bool> rare, std::string input, std::string output){

    std::string filePath = fs::absolute("SV_FLATBUFFERS/SV_SCENES").string();
    const std::string binaryFilePath = filePath+"/"+input;
    const std::string outputFilePath = filePath+"/"+output;

    modelCount = 0;
    fieldCount = 0;

    std::ifstream cleanSceneData(binaryFilePath);
    if(!cleanSceneData.is_open()){
        qFatal()<<"File: "<<binaryFilePath<<" didn't open";
    }

    cleanSceneData >>cleanSceneJSON;
    qDebug()<<QString::fromUtf8(cleanSceneJSON["ObjectTemplateName"].get<std::string>().c_str());

    recursiveFindOfPokemonSceneTable(cleanSceneJSON["Objects"], devId, formId, gender, rare);

    modelCount = 0;
    fieldCount = 0;

    std::ofstream outFile(outputFilePath);
    outFile<<cleanSceneJSON.dump(2);
    outFile.close();
}
