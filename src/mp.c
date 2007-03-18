
/*
 * mp.c
 *
 * Written by Archie Cobbs <archie@freebsd.org>
 * Copyright (c) 1995-1999 Whistle Communications, Inc. All rights reserved.
 * See ``COPYRIGHT.whistle''
 */

#include "ppp.h"
#include "mp.h"
#include "iface.h"
#include "link.h"
#include "fsm.h"
#include "input.h"
#include "util.h"

/*
 * MpInit()
 *
 * Initialize MP state
 */

void
MpInit(Bund b, Link l)
{
  MpState	mp = &b->mp;

  assert(MP_MIN_MRRU <= MP_MAX_MRRU);
  memset(mp, 0, sizeof(*mp));
  mp->self_mrru = l->lcp.want_mrru;
  mp->peer_mrru = l->lcp.peer_mrru;
  mp->self_short_seq = l->lcp.want_shortseq;
  mp->peer_short_seq = l->lcp.peer_shortseq;
}

/*
 * MpStat()
 */

int
MpStat(Context ctx, int ac, char *av[], void *arg)
{
  MpState	mp = &ctx->bund->mp;

  Printf("Multilink self:\r\n");
  Printf("\tMRRU     : %d\r\n"
	"\tShortSeq : %s\r\n",
    mp->self_mrru,
    mp->self_short_seq ? "Yes" : "No");

  Printf("Multilink peer:\r\n");
  Printf("\tMRRU     : %d\r\n"
	 "\tShortSeq : %s\r\n",
    mp->peer_mrru,
    mp->peer_short_seq ? "Yes" : "No");
  return(0);
}

/*
 * MpSetDiscrim()
 *
 * Figure out and set my discriminator
 */

void
MpSetDiscrim(void)
{
  u_int32_t		magic[2];
  struct u_addr		ipaddr;
  struct sockaddr_dl	hwa;

  /* Try Ethernet address first */
  if (GetEther(NULL, &hwa) >= 0) {
    self_discrim.class = DISCRIM_CLASS_802_1;
    memcpy(self_discrim.bytes, LLADDR(&hwa), self_discrim.len = hwa.sdl_alen);
    return;
  }

  /* Then try an IP address */
  if (GetAnyIpAddress(&ipaddr, NULL) >= 0) {
    self_discrim.class = DISCRIM_CLASS_IPADDR;
    memcpy(self_discrim.bytes, &ipaddr.u.ip4, self_discrim.len = sizeof(ipaddr.u.ip4));
    return;
  }

  /* Then just use a coupla magic numbers */
  magic[0] = GenerateMagic();
  magic[1] = GenerateMagic();
  self_discrim.class = DISCRIM_CLASS_MAGIC;
  memcpy(self_discrim.bytes, magic, self_discrim.len = sizeof(magic));
}

/*
 * MpDiscrimEqual()
 *
 * Returns TRUE if the two discriminators are equal.
 */

int
MpDiscrimEqual(Discrim d1, Discrim d2)
{
  if (d1->class != d2->class && d1->len != d2->len)
    return(FALSE);
  return(!memcmp(&d1->bytes, &d2->bytes, d1->len));
}

/*
 * MpDiscrimName()
 */

static const char *
MpDiscrimName(int class)
{
  switch (class)
  {
    case DISCRIM_CLASS_NULL:
      return("NULL");
    case DISCRIM_CLASS_LOCAL:
      return("LOCAL");
    case DISCRIM_CLASS_IPADDR:
      return("IP Address");
    case DISCRIM_CLASS_802_1:
      return("802.1");
    case DISCRIM_CLASS_MAGIC:
      return("Magic");
    case DISCRIM_CLASS_PSN:
      return("PSN");
    default:
      return("???");
  }
}

/*
 * MpDiscrimText()
 */

char *
MpDiscrimText(Discrim dis)
{
  int		k;
  static char	line[100];

  snprintf(line, sizeof(line), "[%s]", MpDiscrimName(dis->class));
  for (k = 0; k < dis->len && k < sizeof(dis->bytes); k++)
    snprintf(line + strlen(line),
      sizeof(line) - strlen(line), " %02x", dis->bytes[k]);
  return(line);
}


