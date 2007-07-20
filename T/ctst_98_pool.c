#include "tst.h"

static const char *rf = __FILE__;

int tst(void)
{
  Ustr_pool *pool = ustr_pool_make_pool();
  Ustr_pool *p1 = NULL;
  Ustr_pool *p2 = NULL;
  Ustr_pool *p3 = NULL;
  Ustrp *sp1 = ustrp_dup_empty(pool);
  Ustrp *sp2 = USTRP1(\x2, "s2");
  char buf_spa[1024];
  Ustrp *spa = USTRP_SC_INIT_AUTO(buf_spa, USTR_FALSE, 0);
  size_t off = 0;
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_si_base *)pool)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)pool)->beg->ptr == sp1);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->beg->next);
#endif
  
  ASSERT(pool->pool_sys_realloc(pool, NULL, 0, 0));
  ASSERT(pool->pool_sys_realloc(pool, NULL, 0, 1));
  
  ASSERT( ustrp_size_alloc(sp1));
  ASSERT( ustrp_size_alloc(sp2) >= 4);
  ASSERT(spa == (void *)buf_spa);
  ASSERT(!ustrp_alloc(spa));
  ASSERT( ustrp_owner(spa));
  ASSERT( ustrp_fixed(spa));
  ASSERT(!ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_size_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  ASSERT( ustrp_set_rep_chr(pool, &spa, '-', 2000));
  ASSERT(!ustrp_enomem(spa));
  ASSERT(spa != (void *)buf_spa);
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));

  ASSERT((spa = USTRP_SC_INIT_AUTO(buf_spa, USTR_TRUE, 0)));
  ASSERT( ustrp_size_alloc(sp1));
  ASSERT( ustrp_size_alloc(sp2) >= 4);
  ASSERT(spa == (void *)buf_spa);
  ASSERT(!ustrp_alloc(spa));
  ASSERT( ustrp_owner(spa));
  ASSERT( ustrp_fixed(spa));
  ASSERT( ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_size_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  ASSERT(!ustrp_set_rep_chr(pool, &spa, '-', 2000));
  ASSERT( ustrp_enomem(spa));
  ASSERT(spa == (void *)buf_spa);
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));
  
  ASSERT((spa = ustrp_init_alloc(buf_spa, sizeof(buf_spa), sizeof(buf_spa),
                                 1, 1, 0, 0)));
  ASSERT(spa == (void *)buf_spa);
  ASSERT( ustrp_alloc(spa)); /* it _thinks so_ */
  ASSERT( ustrp_owner(spa));
  ASSERT(!ustrp_fixed(spa));
  ASSERT(!ustrp_limited(spa));
  ASSERT(!ustrp_ro(spa));
  ASSERT((ustrp_size(spa) + ustrp_size_overhead(spa)) == sizeof(buf_spa));
  ASSERT(ustrp_size_alloc(spa) == sizeof(buf_spa));

  /* this is a huge hack based on pool_free() being a noop when it wasn't
   * the last ptr. */
  ASSERT( ustrp_set_rep_chr(pool, &spa, '-', 2000));
  ASSERT(!ustrp_enomem(spa));
  ASSERT(spa != (void *)buf_spa);
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));
  
  ASSERT((spa = ustrp_dup_undef(pool, 0)));
  ASSERT(!ustrp_alloc(spa));
  ASSERT(!ustrp_owner(spa));
  ASSERT( ustrp_ro(spa));
  ASSERT(ustrp_set_empty(pool, &spa));
  ASSERT(ustrp_set(pool, &spa, spa));

  ASSERT((spa = ustrp_dup_undef(pool, 0)));
  ASSERT(!ustrp_alloc(spa));
  ASSERT(!ustrp_owner(spa));
  ASSERT( ustrp_ro(spa));
  ASSERT(ustrp_set(pool, &spa, spa));
  if (!USTR_DEBUG)
    ASSERT(!ustrp_set_subustrp(pool, &spa, USTRP1(\x4, "abcd"), 1, 8));
  ASSERT(ustrp_set_subustrp(pool, &spa, USTRP1(\x4, "abcd"), 1, 4));

  ASSERT((spa = ustrp_dupx_empty(pool, 0, 1, 0, 0)));
  ASSERT(ustrp_dup(pool, spa));
  ASSERT(ustrp_set(pool, &spa, USTRP1(\x4, "abcd")));
  
  ASSERT(pool);
  ASSERT(!ustrp_ro(sp1));
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT((sp1 = ustrp_dup_fmt_lim(pool, 4000, "%.5000x", 0xdeadbeef)));
  ASSERT(!ustrp_ro(sp1));
  ASSERT(ustrp_len(sp1) == 4000);
  ASSERT((sp1 = ustrp_dup_fmt(pool,           "%.5000x", 0xdeadbeef)));
  ASSERT(ustrp_len(sp1) == 5000);

  ustrp_sc_del(pool, &sp1);
  
  ustr_pool_clear(pool);
  
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT(ustrp_len(sp2) == 2);
  ASSERT(ustrp_assert_valid_subustrp(sp2, 1, 1));
  ASSERT(ustrp_assert_valid_subustrp(sp2, 1, 2));
  ASSERT(ustrp_assert_valid_subustrp(sp2, 2, 1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 1, 1));
  ASSERT(ustrp_len(sp1) == 1);
  if (!USTR_DEBUG)
    ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 3, 1));
  ASSERT(ustrp_len(sp1) == 1);
  if (!USTR_DEBUG)
    ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 1, 3));
  ASSERT(ustrp_len(sp1) == 1);
  ASSERT(ustrp_add_subustrp(pool, &sp1, sp2, 1, 2));
  ASSERT(ustrp_len(sp1) == 3);
  ASSERT(ustrp_add_subustrp(pool, &sp1, sp1, 1, 3));
  ASSERT(ustrp_len(sp1) == 6);
  ASSERT(ustrp_add_subustrp(pool, &sp1, sp1, 1, 3));
  ASSERT(ustrp_len(sp1) == 9);
  
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 1, 0)));
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 0)));
  ASSERT(ustrp_len(sp1) == 0);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 1, 2)));
  ASSERT(ustrp_len(sp1) == 2);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 2)));
  ASSERT(ustrp_len(sp1) == 2);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 2, 1)));
  ASSERT(ustrp_len(sp1) == 1);
  ASSERT((sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 2, 1)));
  ASSERT(ustrp_len(sp1) == 1);

  if (!USTR_DEBUG)
  {
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 3, 1)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 3, 1)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 1, 3)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 1, 3)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 0, 0, 0, 0, sp2, 2, 2)));
  ASSERT(!(sp1 = ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 2, 2)));
  }
  
  ustrp_sc_free2(pool, &sp1, USTRP(""));

  if (!USTR_DEBUG)
  {
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 1, 3));
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 3, 1));
  ASSERT(!ustrp_add_subustrp(pool, &sp1, sp2, 2, 2));
  }
  
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x1, "2"));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "22"));
  ASSERT( ustrp_add_rep_chr(pool, &sp1, 'x', 18));
  ASSERT_PEQ(sp1, USTRP1(\x14, "22xxxxxxxxxxxxxxxxxx"));

  /* throw away a Ustrp, so sp1 isn't the last */
  ASSERT(ustrp_dupx_subustrp(pool, 1, 1, 1, 1, sp2, 2, 1));

  ASSERT(ustrp_del(pool, &sp1, 15));
  ASSERT_PEQ(sp1, USTRP1(\x5, "22xxx"));
  ASSERT( ustrp_add_rep_chr(pool, &sp1, 'y', 13));
  ASSERT_PEQ(sp1, USTRP1(\x12, "22xxxyyyyyyyyyyyyy"));

  /* test some of the data ... */

  /* spn */
  ASSERT(ustrp_spn_chr_fwd(sp1, 0, 'x') == 0);
  ASSERT(ustrp_spn_chr_fwd(sp1, 0, '2') == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "2") == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "22") == 2);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "2x") == 5);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "x2") == 5);
  ASSERT(ustrp_spn_cstr_fwd(sp1, 0, "x2y") == 18);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x2, "22")) == 2);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x2, "2x")) == 5);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x2, "x2")) == 5);
  ASSERT(ustrp_spn_fwd(sp1, 0, USTRP1(\x3, "x2y")) == 18);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "x2") == 0);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "x2y") == 18);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "2y") == 13);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "y") == 13);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x2, "x2")) == 0);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x3, "x2y")) == 18);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x2, "2y")) == 13);
  ASSERT(ustrp_spn_rev(sp1, 0, USTRP1(\x1, "y")) == 13);
  ASSERT(ustrp_spn_chr_rev(sp1, 0, 'x') == 0);
  ASSERT(ustrp_spn_chr_rev(sp1, 0, 'y') == 13);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "yx") == 16);
  ASSERT(ustrp_spn_cstr_rev(sp1, 0, "xy") == 16);

  /* cspn */
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, 'y') == 5);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, 'x') == 2);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, '2') == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "2") == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "y") == 5);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "22") == 0);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "x") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "xx") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "yx") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "xy") == 2);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "yz") == 5);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "zy") == 5);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "2")) == 0);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "y")) == 5);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "22")) == 0);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "x")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "xx")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "yx")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "xy")) == 2);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "yz")) == 5);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x2, "zy")) == 5);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "2x") == 13);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "x2") == 13);
  ASSERT(ustrp_cspn_chr_rev(sp1, 0, '2') == 16);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "2") == 16);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "2a") == 16);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x2, "2x")) == 13);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x2, "x2")) == 13);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x1, "2")) == 16);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x2, "2a")) == 16);
  ASSERT(ustrp_cspn_chr_fwd(sp1, 0, 'a') == 18);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "a") == 18);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "a") == 18);
  ASSERT(ustrp_cspn_chr_rev(sp1, 0, 'a') == 18);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x1, "a")) == 18);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x1, "a")) == 18);
  ASSERT(ustrp_cspn_cstr_fwd(sp1, 0, "abc") == 18);
  ASSERT(ustrp_cspn_cstr_rev(sp1, 0, "abc") == 18);
  ASSERT(ustrp_cspn_fwd(sp1, 0, USTRP1(\x3, "abc")) == 18);
  ASSERT(ustrp_cspn_rev(sp1, 0, USTRP1(\x3, "abc")) == 18);
  
  /* srch */
  ASSERT(ustrp_srch_cstr_fwd(sp1, 0, "xy") == 5);
  ASSERT(ustrp_srch_cstr_rev(sp1, 0, "xy") == 5);
  ASSERT(ustrp_srch_fwd(sp1, 0, USTRP1(\x2, "xy")) == 5);
  ASSERT(ustrp_srch_rev(sp1, 0, USTRP1(\x2, "xy")) == 5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "xy"), 1, 2) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "xy"), 1, 1) ==  3);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "xy"), 1, 1) ==  5);
  ASSERT(ustrp_srch_subustrp_fwd(sp1, 0, USTRP1(\x2, "xy"), 2, 1) ==  6);
  ASSERT(ustrp_srch_subustrp_rev(sp1, 0, USTRP1(\x2, "xy"), 2, 1) == 18);
  ASSERT(ustrp_srch_chr_fwd(sp1, 0, 'x') ==  3);
  ASSERT(ustrp_srch_chr_rev(sp1, 0, 'x') ==  5);
  ASSERT(ustrp_srch_chr_fwd(sp1, 0, 'y') ==  6);
  ASSERT(ustrp_srch_chr_rev(sp1, 0, 'y') == 18);
  
  /* do some more stuff... */
  ustrp_free(pool, sp1);
  sp1 = USTRP1(\x1, "c");
  
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 1, 1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "cs"));
  ASSERT(ustrp_setf_share(sp1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "cs2"));
  ustrp_sc_free2(pool, &sp1, ustrp_dupx(pool, 1, 1, 1, 1, USTRP("")));
  ASSERT_PEQ(sp1, USTRP(""));
  ASSERT(ustrp_setf_share(sp1));
  ASSERT(!ustrp_owner(sp1));
  ASSERT( ustrp_shared(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT(!ustrp_realloc(pool, &sp1, 0));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT( ustrp_add_subustrp(pool, &sp1, sp2, 2, 1));
  ASSERT( ustrp_owner(sp1));
  ASSERT(!ustrp_shared(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT(!ustrp_realloc(pool, &sp1, 1));
  ASSERT( ustrp_realloc(pool, &sp1, 2));
  ASSERT( ustrp_realloc(pool, &sp1, 2));
  ASSERT_PEQ(sp1, USTRP1(\x2, "22"));
  
  ustrp_sc_free2(pool, &sp1, USTRP(""));

  /* test subpool API */
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(!((struct Ustr__pool_si_base *)pool)->sbeg);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->next);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->prev);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->base);
#endif
  
  ASSERT((p1 = ustr_pool_make_subpool(pool)));
  ASSERT((p2 = ustr_pool_make_subpool(pool)));
  ASSERT((p3 = ustr_pool_make_subpool(pool)));

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(&((struct Ustr__pool_si_base *)pool)->sbeg->cbs == p3);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->next);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->prev);
  ASSERT(!((struct Ustr__pool_si_base *)pool)->base);

  ASSERT(!((struct Ustr__pool_si_base *)p3)->sbeg);
  ASSERT(&((struct Ustr__pool_si_base *)p3)->next->cbs == p2);
  ASSERT(!((struct Ustr__pool_si_base *)p3)->prev);
  ASSERT(&((struct Ustr__pool_si_base *)p3)->base->cbs == pool);
  ASSERT(!((struct Ustr__pool_si_base *)p2)->sbeg);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->next->cbs == p1);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->prev->cbs == p3);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->base->cbs == pool);
  ASSERT(!((struct Ustr__pool_si_base *)p1)->sbeg);
  ASSERT(!((struct Ustr__pool_si_base *)p1)->next);
  ASSERT(&((struct Ustr__pool_si_base *)p1)->prev->cbs == p2);
  ASSERT(&((struct Ustr__pool_si_base *)p1)->base->cbs == pool);
#endif
  
  ustr_pool_clear(p2);
  ustr_pool_clear(p1);
  ustr_pool_clear(p3);

  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_si_base *)p1)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)p2)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)p3)->beg);
#endif
  
  ustr_pool_clear(p2);

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT( ((struct Ustr__pool_si_base *)p1)->beg);
  ASSERT(!((struct Ustr__pool_si_base *)p2)->beg);
  ASSERT( ((struct Ustr__pool_si_base *)p3)->beg);
#endif
  
  ustr_pool_free(p3);
  
  ASSERT((p3 = ustr_pool_make_subpool(p2)));
  
#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(!((struct Ustr__pool_si_base *)p1)->next);
  ASSERT(&((struct Ustr__pool_si_base *)p1)->prev->cbs == p2);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->next->cbs == p1);
  ASSERT(!((struct Ustr__pool_si_base *)p2)->prev);
  ASSERT(&((struct Ustr__pool_si_base *)p2)->sbeg->cbs == p3);
  ASSERT(!((struct Ustr__pool_si_base *)p3)->next);
  ASSERT(!((struct Ustr__pool_si_base *)p3)->prev);
  ASSERT(&((struct Ustr__pool_si_base *)p3)->base->cbs == p2);
#endif
  
  ASSERT(ustrp_dup_undef(p3, 2));
  ASSERT(ustrp_dup_undef(p3, 2));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 4));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  
  ASSERT((p3 = ustr_pool_make_subpool(pool)));
  
  ustr_pool_free(p2);
  
  ASSERT((p1 = ustr_pool_make_subpool(p3)));
  ASSERT((p2 = ustr_pool_make_subpool(p3)));
  ASSERT((p3 = ustr_pool_make_subpool(p3)));
  
  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p1, 2));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p2, 4));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));
  ASSERT(ustrp_dup_undef(p3, 8));

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(p1 && ((struct Ustr__pool_si_base *)p1)->beg &&
         ((struct Ustr__pool_si_base *)p1)->beg->ptr);
  ASSERT(p2 && ((struct Ustr__pool_si_base *)p2)->beg &&
         ((struct Ustr__pool_si_base *)p2)->beg->ptr);
  ASSERT(p3 && ((struct Ustr__pool_si_base *)p3)->beg &&
         ((struct Ustr__pool_si_base *)p3)->beg->ptr);
#endif
  
  ustr_pool_clear(pool);

#if USTR_CONF_INCLUDE_INTERNAL_HEADERS
  ASSERT(p1 && !((struct Ustr__pool_si_base *)p1)->beg);
  ASSERT(p2 && !((struct Ustr__pool_si_base *)p2)->beg);
  ASSERT(p3 && !((struct Ustr__pool_si_base *)p3)->beg);
#endif
  
  ASSERT_PEQ(USTRP1(\x8, "1234567z"), ustrp_dup_cstr(pool, "1234567z"));
  ASSERT(ustrp_assert_valid(sp1));
  ASSERT((sp1 = ustrp_dupx_cstr(pool, 0, 0, USTR_TRUE, USTR_TRUE, "1234567z")));
  ASSERT_PEQ(USTRP1(\x8, "1234567z"), sp1);
  ASSERT(!ustrp_sized(sp1));
  ASSERT(!ustrp_setf_share(sp1));
  ASSERT( ustrp_setf_owner(sp1));
  ASSERT(ustrp_exact(sp1));
  ASSERT(ustrp_size(sp1) == ustrp_len(sp1));
  ASSERT(ustrp_size_alloc(sp1) == ustrp_len(sp1) + ustrp_size_overhead(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT( ustrp_setf_enomem_clr(sp1));
  ASSERT(!ustrp_enomem(sp1));
  ASSERT( ustrp_setf_enomem_err(sp1));
  ASSERT( ustrp_enomem(sp1));
  ASSERT(ustrp_add_cstr(pool, &sp1, "abcd"));
  ASSERT_PEQ(USTRP1(\xc, "1234567zabcd"), sp1);

  ustrp_sc_free2(pool, &sp1, ustrp_dupx_cstr(pool, 1, 1, 1, 1, "1234abcd"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "1234abcd"));
  ASSERT(ustrp_enomem(sp1));
  ASSERT(ustrp_sized(sp1));
  ASSERT(ustrp_exact(sp1));
  ASSERT(ustrp_setf_enomem_clr(sp1));

  ASSERT(ustrp_sc_toupper(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x8, "1234ABCD"));
  ASSERT(ustrp_sc_tolower(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x8, "1234abcd"));

  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x8, "dcba4321"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x7, "234abcd"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x6, "cba432"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x5, "34abc"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x4, "ba43"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x3, "4ab"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x2, "a4"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP1(\x1, "a"));
  ASSERT(ustrp_del(pool, &sp1, 1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP(""));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT(ustrp_sc_reverse(pool, &sp1));
  ASSERT_PEQ(sp1, USTRP(""));  
  
  ustr_pool_clear(pool);

  sp1 = USTRP("");
  ASSERT_PEQ(sp1, USTRP(""));  
  ASSERT(ustrp_ins(pool, &sp1, 0, USTRP("")));
  ASSERT(ustrp_ins(pool, &sp1, 0, USTRP("")));
  ASSERT_PEQ(sp1, USTRP(""));  
  ASSERT(ustrp_ins(pool, &sp1, 0, USTRP1(\4, "abcd")));
  ASSERT_PEQ(sp1, USTRP1(\4, "abcd"));
  ASSERT(ustrp_ins(pool, &sp1, 2, USTRP1(\1, "x")));
  ASSERT_PEQ(sp1, USTRP1(\5, "abxcd"));
  ASSERT(ustrp_ins_cstr(pool, &sp1, 2, "y"));
  ASSERT_PEQ(sp1, USTRP1(\6, "abyxcd"));
  ASSERT(ustrp_ins_buf(pool, &sp1, 1, "Zabcd", 1));
  ASSERT_PEQ(sp1, USTRP1(\7, "aZbyxcd"));
  ASSERT(ustrp_ins_rep_chr(pool, &sp1, 0, '-', 2));
  ASSERT_PEQ(sp1, USTRP1(\x9, "--aZbyxcd"));
  
  ASSERT(ustrp_sc_sub_rep_chr(pool, &sp1, 1, 4, '=', 2));
  ASSERT_PEQ(sp1, USTRP1(\7, "==byxcd"));
  ASSERT(ustrp_sc_sub_cstr(pool, &sp1, 6, 2, "___"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "==byx___"));
  ASSERT(ustrp_sc_sub_buf(pool, &sp1, 1, 2, "___", 1));
  ASSERT_PEQ(sp1, USTRP1(\7, "_byx___"));
  ASSERT(ustrp_sc_sub_fmt(pool, &sp1, 2, 6, "%2s%4d%10s", "x", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\x11, "_ x   4         y"));
  ASSERT(ustrp_sc_sub_fmt_lim(pool, &sp1, 2, 16, 1, "%s%4d%10s", "!", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\2, "_!"));
  ASSERT(ustrp_ins_fmt(pool, &sp1, 1, "%s%2d%s", "x", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\6, "_x 4y!"));
  ASSERT(ustrp_ins_fmt_lim(pool, &sp1, 0, 2, "%s%2d%s", "x", 4, "y"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "x _x 4y!"));

  ASSERT(ustrp_sub_rep_chr(pool, &sp1, 1, '=', 2));
  ASSERT_PEQ(sp1, USTRP1(\x8, "==_x 4y!"));
  ASSERT(ustrp_sub_cstr(pool, &sp1, 4, "abcd"));
  ASSERT_PEQ(sp1, USTRP1(\x8, "==_abcd!"));
  ASSERT(ustrp_sub_buf(pool, &sp1, 2, "___abcd", 3));
  ASSERT_PEQ(sp1, USTRP1(\x8, "=___bcd!"));
  ASSERT(ustrp_sub(pool, &sp1, 1, USTRP1(\3, "---")));
  ASSERT_PEQ(sp1, USTRP1(\x8, "---_bcd!"));
  ASSERT(ustrp_sc_sub(pool, &sp1, 2, 4, USTRP1(\3, "===")));
  ASSERT_PEQ(sp1, USTRP1(\7, "-===cd!"));

  ASSERT(ustrp_sub_fmt(pool, &sp1, 5, "%s", "xyz"));
  ASSERT_PEQ(sp1, USTRP1(\7, "-===xyz"));
  ASSERT(ustrp_sub_subustrp(pool, &sp1, 2, USTRP1(\4, "1234"), 2, 2));
  ASSERT_PEQ(sp1, USTRP1(\7, "-23=xyz"));
  ASSERT(ustrp_sc_sub_subustrp(pool, &sp1, 2, 4, USTRP1(\4, "12*&"), 3, 2));
  ASSERT_PEQ(sp1, USTRP1(\5, "-*&yz"));
  ASSERT(ustrp_sub_fmt_lim(pool, &sp1, 5, 2, "%s", "123456789"));
  ASSERT_PEQ(sp1, USTRP1(\6, "-*&y12"));
  
  ustrp_sc_free2(pool, &sp1, USTRP1(\x10, "123456789 123456"));
  ASSERT(!ustrp_replace(pool, &sp1, USTRP1(\1, "!"), USTRP1(\4, "abcd"), 0));
  ASSERT(!ustrp_replace_buf(pool, &sp1, "1", 2, "abcd", 4, 0));
  ASSERT_PEQ(sp1, USTRP1(\x10, "123456789 123456"));
  ASSERT(ustrp_replace_cstr(pool, &sp1, "123", "xyz", 1));
  ASSERT_PEQ(sp1, USTRP1(\x10, "xyz456789 123456"));
  ASSERT(ustrp_replace_cstr(pool, &sp1, "123", "xyz", 1));
  ASSERT_PEQ(sp1, USTRP1(\x10, "xyz456789 xyz456"));
  ASSERT(!ustrp_replace_cstr(pool, &sp1, "123", "xyz", 1));
  ASSERT_PEQ(sp1, USTRP1(\x10, "xyz456789 xyz456"));

  off = 0;
  ASSERT_PEQ(ustrp_split_cstr(pool, sp1, &off, "xyz", 0),USTRP1(\7, "456789 "));
  ASSERT_PEQ(ustrp_split_cstr(pool, sp1, &off, "xyz", 0),USTRP1(\3, "456"));
  ASSERT(!ustrp_split_cstr(pool, sp1, &off, "xyz", 0));
  ASSERT(!ustrp_split_cstr(pool, sp1, &off, "xyz", 0));
  off = 0;
  ASSERT_PEQ(ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), 0),
             USTRP1(\7, "456789 "));
  ASSERT_PEQ(ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), 0),
             USTRP1(\3, "456"));
  ASSERT(!ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), 0));
  ASSERT(!ustrp_split(pool, sp1, &off, USTRP1(\3, "xyz"), 0));
  
  ustr_pool_free(pool);
  ustr_pool_free(NULL);
  
  return (EXIT_SUCCESS);
}