
/*
 * proto.c
 *
 * Written by Archie Cobbs <archie@freebsd.org>
 * Copyright (c) 1995-1999 Whistle Communications, Inc. All rights reserved.
 * See ``COPYRIGHT.whistle''
 */

#include "ppp.h"

/*
 * DEFINITIONS
 */

  /* Protocol number <-> name mapping */
  struct protoname {
    u_short	proto;
    const char	*name;
  };

/*
 * INTERNAL VARIABLES
 */

  /* These are protocols we use and keep stats on */
  static const struct protoname statProtos[] = {
    { 0,		"Unknown" },
    { PROTO_IP,		"IP" },
    { PROTO_VJUNCOMP,	"VJUNCOMP" },
    { PROTO_VJCOMP,	"VJCOMP" },
    { PROTO_IPV6,	"IPv6" },
    { PROTO_MP,		"MP" },
    { PROTO_IPCP,	"IPCP" },
    { PROTO_IPV6CP,	"IPV6CP" },
    { PROTO_ICOMPD,	"ICOMPD" },
    { PROTO_COMPD,	"COMPD" },
    { PROTO_ICCP,	"ICCP" },
    { PROTO_CCP,	"CCP" },
    { PROTO_LCP,	"LCP" },
    { PROTO_PAP,	"PAP" },
    { PROTO_LQR,	"LQR" },
    { PROTO_CHAP,	"CHAP" },
    { PROTO_ICRYPT,	"ICRYPT" },
    { PROTO_CRYPT,	"CRYPT" },
    { PROTO_IECP,	"IECP" },
    { PROTO_ECP,	"ECP" },
    { PROTO_SPAP,	"SPAP" },
    { PROTO_ATCP,	"ATCP" },
    { PROTO_EAP,	"EAP" },
  };
  #define NUM_STAT_PROTOCOLS	(sizeof(statProtos) / sizeof(*statProtos))

/*
 * These are other protocols we recognize but don't use
 * Reference: http://www.iana.org/assignments/ppp-numbers/ppp-numbers.xhtml
 */

  static const struct protoname protoNames[] = {
#ifdef PROTO_NAME_LIST

/* Network layer protocols */
    { 0x0001,	"Padding Protocol" },
    { 0x0002,	"ROHC small-CID" },
    { 0x0005,	"ROHC large-CID" },
    { 0x0021,	"Internet Protocol" },
    { 0x0023,	"OSI Network Layer" },
    { 0x0025,	"Xerox NS IDP" },
    { 0x0027,	"DECnet Phase IV" },
    { 0x0029,	"Appletalk" },
    { 0x002b,	"Novell IPX" },
    { 0x002d,	"Van Jacobson Compressed TCP/IP" },
    { 0x002f,	"Van Jacobson Uncompressed TCP/IP" },
    { 0x0031,	"Bridging PDU" },
    { 0x0033,	"Stream Protocol (ST-II)" },
    { 0x0035,	"Banyan Vines" },
    { 0x0037,	"reserved (until 1993)" },
    { 0x0039,	"AppleTalk EDDP" },
    { 0x003b,	"AppleTalk SmartBuffered" },
    { 0x003d,	"Multi-Link" },
    { 0x003f,	"NETBIOS Framing" },
    { 0x0041,	"Cisco Systems" },
    { 0x0043,	"Ascom Timeplex" },
    { 0x0045,	"Fujitsu Link Backup and Load Balancing (LBLB)" },
    { 0x0047,	"DCA Remote Lan" },
    { 0x0049,	"Serial Data Transport Protocol (PPP-SDTP)" },
    { 0x004b,	"SNA over 802.2" },
    { 0x004d,	"SNA" },
    { 0x004f,	"IP6 Header Compression" },
    { 0x0051,	"KNX Bridging Data" },
    { 0x0053,	"Encryption" },
    { 0x0055,	"Individual Link Encryption" },
    { 0x0057,	"Internet Protocol version 6" },
    { 0x0059,	"PPP Muxing (RFC 3153)" },
    { 0x005b,	"Vendor-Specific Network Protocol" },
    { 0x005d,	"TRILL Network Protocol (TNP)" },
    { 0x0061,	"RTP IPHC Full Header" },
    { 0x0063,	"RTP IPHC Compressed TCP" },
    { 0x0065,	"RTP IPHC Compressed Non TCP" },
    { 0x0067,	"RTP IPHC Compressed UDP 8" },
    { 0x0069,	"RTP IPHC Compressed RTP 8" },
    { 0x006f,	"Stampede Bridging" },
    { 0x0071,	"BAP Bandwidth Allocation Protocol" },
    { 0x0073,	"MP+ Protocol" },
    { 0x007d,	"reserved (Control Escape) (RFC 1661)" },
    { 0x007f,	"reserved (compression inefficient) (RFC 1662)" },
    { 0x0081,	"Unassigned" },
    { 0x0083,	"Unassigned" },
    { 0x00c1,	"NTCITS IPI" },
    { 0x00cf,	"Reserved (PPP NLPID)" },
    { 0x00fb,	"Single link compression in multilink" },
    { 0x00fd,	"Compressed datagram" },
    { 0x00ff,	"Reserved (compression inefficient)" },

    { 0x0201,	"802.1d Hello Packets (RFC 1220)" },
    { 0x0203,	"IBM Source Routing BPDU" },
    { 0x0205,	"DEC LANBridge100 Spanning Tree" },
    { 0x0207,	"Cisco Discovery Protocol" },
    { 0x0209,	"Netcs Twin Routing" },
    { 0x020b,	"STP - Scheduled Transfer Protocol" },
    { 0x020d,	"EDP - Extreme Discovery Protocol" },
    { 0x0211,	"Optical Supervisory Channel Protocol (OSCP)" },
    { 0x0213,	"Optical Supervisory Channel Protocol (OSCP)" },
    { 0x0231,	"Luxcom" },
    { 0x0233,	"Sigma Network Systems" },
    { 0x0235,	"Apple Client Server Protocol" },
    { 0x0281,	"MPLS Unicast" },
    { 0x0283,	"MPLS Multicast" },
    { 0x0285,	"IEEE p1284.4 standard - data packets" },
    { 0x0287,	"ETSI TETRA Network Protocol Type 1" },
    { 0x0289,	"Multichannel Flow Treatment Protocol" },

    { 0x2063,	"RTP IPHC Compressed TCP No Delta" },
    { 0x2065,	"RTP IPHC Context State" },
    { 0x2067,	"RTP IPHC Compressed UDP 16" },
    { 0x2069,	"RTP IPHC Compressed RTP 16" },

    { 0x4001,	"Cray Communications Control Protocol" },
    { 0x4003,	"CDPD Mobile Network Registration Protocol" },
    { 0x4005,	"Expand accelerator protocol" },
    { 0x4007,	"ODSICP NCP" },
    { 0x4009,	"DOCSIS DLL" },
    { 0x400b,	"Cetacean Network Detection Protocol" },
    { 0x4021,	"Stacker LZS" },
    { 0x4023,	"RefTek Protocol" },
    { 0x4025,	"Fibre Channel" },
    { 0x4027,	"OpenDOF" },
    { 0x405b,	"Vendor-Specific Protocol (VSP)" },
    { 0x405d,	"TRILL Link State Protocol (TLSP)" },

/* Network layer control protocols */

    { 0x8021,	"Internet Protocol Control Protocol" },
    { 0x8023,	"OSI Network Layer Control Protocol" },
    { 0x8025,	"Xerox NS IDP Control Protocol" },
    { 0x8027,	"DECnet Phase IV Control Protocol" },
    { 0x8029,	"Appletalk Control Protocol" },
    { 0x802b,	"Novell IPX Control Protocol" },
    { 0x802d,	"Reserved" },
    { 0x802f,	"Reserved" },
    { 0x8031,	"Bridging NCP" },
    { 0x8033,	"Stream Protocol Control Protocol" },
    { 0x8035,	"Banyan Vines Control Protocol" },
    { 0x8037,	"Unassigned" },
    { 0x8039,	"Reserved" },
    { 0x803b,	"Reserved" },
    { 0x803d,	"Multi-Link Control Protocol" },
    { 0x803f,	"NETBIOS Framing Control Protocol" },
    { 0x8041,	"Cisco Systems Control Protocol" },
    { 0x8043,	"Ascom Timeplex" },
    { 0x8045,	"Fujitsu LBLB Control Protocol" },
    { 0x8047,	"DCA Remote Lan Network Control Protocol (RLNCP)" },
    { 0x8049,	"Serial Data Control Protocol (PPP-SDCP)" },
    { 0x804b,	"SNA over 802.2 Control Protocol" },
    { 0x804d,	"SNA Control Protocol" },
    { 0x804f,	"IP6 Header Compression Control Protocol" },
    { 0x8051,	"KNX Bridging Control Protocol" },
    { 0x8053,	"Encryption Control Protocol" },
    { 0x8055,	"Individual Link Encryption Control Protocol" },
    { 0x8057,	"IPv6 PPP Control Protocol" },
    { 0x8059,	"PPP Muxing Control Protocol" },
    { 0x805b,	"Vendor-Specific Network Control Protocol (VSNCP)" },
    { 0x805d,	"TRILL Network Control Protocol (TNCP)" },
    { 0x806f,	"Stampede Bridging Control Protocol" },
    { 0x8071,	"BACP Bandwidth Allocation Control Protocol" },
    { 0x8073,	"MP+ Control Protocol" },
    { 0x807d,	"Not Used - reserved" },
    { 0x8081,	"Unassigned" },
    { 0x8083,	"Unassigned" },
    { 0x80c1,	"NTCITS IPI Control Protocol" },
    { 0x80cf,	"Not Used - reserved" },
    { 0x80fb,	"single link compression in multilink control" },
    { 0x80fd,	"Compression Control Protocol" },
    { 0x80ff,	"Not Used - reserved" },

    { 0x8207,	"Cisco Discovery Protocol Control" },
    { 0x8209,	"Netcs Twin Routing" },
    { 0x820b,	"STP - Control Protocol" },
    { 0x820d,	"EDPCP - Extreme Discovery Protocol Ctrl Prtcl" },
    { 0x8235,	"Apple Client Server Protocol Control" },
    { 0x8281,	"MPLS Control Protocol (RFC 3032)" },
    { 0x8283,	"Tag Switching - Multicast" },
    { 0x8285,	"IEEE p1284.4 standard - Protocol Control" },
    { 0x8287,	"ETSI TETRA NSP1 Control Protocol" },
    { 0x8289,	"Multichannel Flow Treatment Protocol" },

/* Link layer control protocols */

    { 0xc021,	"Link Control Protocol" },
    { 0xc023,	"Password Authentication Protocol" },
    { 0xc025,	"Link Quality Report" },
    { 0xc027,	"Shiva Password Authentication Protocol" },
    { 0xc029,	"CallBack Control Protocol (CBCP)" },
    { 0xc02b,	"BACP Bandwidth Allocation Control Protocol (RFC 2125)" },
    { 0xc02d,	"BAP (RFC 2125)" },
    { 0xc05b,	"Vendor-Specific Authentication Protocol (RFC 3772)" },
    { 0xc081,	"Container Control Protocol", },
    { 0xc223,	"Challenge Handshake Authentication Protocol" },
    { 0xc225,	"RSA Authentication Protocol" },
    { 0xc227,	"Extensible Authentication Protocol" },
    { 0xc229,	"Mitsubishi Security Info Exch Ptcl (SIEP)" },
    { 0xc26f,	"Stampede Bridging Authorization Protocol" },
    { 0xc281,	"Proprietary Authentication Protocol", },
    { 0xc283,	"Proprietary Authentication Protocol", },
    { 0xc481,	"Proprietary Node ID Authentication Protocol", },
#endif
  };
  #define NUM_PROTO_NAMES	(sizeof(protoNames) / sizeof(*protoNames))

/*
 * INTERNAL FUNCTIONS
 */

  static int	ProtoNameCmp(const void *v1, const void *v2);

/*
 * ProtoName()
 * XXX For unknown protocols this function return pointer 
 * on the static variable. It is not good, but I think 
 * it should not create troubles as it is used only for logging.
 */

const char *
ProtoName(int proto)
{
  int			k;
  static char		buf[20];
  struct protoname	key, *pn;

  /* First check our stat list for known short names */
  for (k = 0; k < NUM_STAT_PROTOCOLS; k++) {
    if (proto == statProtos[k].proto)
      return(statProtos[k].name);
  }

  /* Now look in list of all defined protocols */
  key.proto = proto;
  if ((pn = bsearch(&key, protoNames,
      NUM_PROTO_NAMES, sizeof(*pn), ProtoNameCmp)) != NULL)
    return(pn->name);

  /* Return hex value */
  buf[19] = 0;
  sprintf(buf, "0x%04x", proto);
  return(buf);
}

/*
 * ProtoNameCmp()
 */

static int
ProtoNameCmp(const void *v1, const void *v2)
{
  struct protoname	*const p1 = (struct protoname *) v1;
  struct protoname	*const p2 = (struct protoname *) v2;

  return(p1->proto - p2->proto);
}


