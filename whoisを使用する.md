# whois を使用する

- 1.Windows ターミナルを起動

- 2.WSL を起動

* 3.whois をインストール

```
  sudo apt install whois
```

- 4. whois コマンドでドメイン情報を確認

```
whois example.com
```

```
   Domain Name: EXAMPLE.COM
   Registry Domain ID: 2336799_DOMAIN_COM-VRSN
   Registrar WHOIS Server: whois.iana.org
   Registrar URL: http://res-dom.iana.org
   Updated Date: 2022-08-14T07:01:31Z
   Creation Date: 1995-08-14T04:00:00Z
   Registry Expiry Date: 2023-08-13T04:00:00Z
   Registrar: RESERVED-Internet Assigned Numbers Authority
   Registrar IANA ID: 376
   Registrar Abuse Contact Email:
   Registrar Abuse Contact Phone:
   Domain Status: clientDeleteProhibited https://icann.org/epp#clientDeleteProhibited
   Domain Status: clientTransferProhibited https://icann.org/epp#clientTransferProhibited
   Domain Status: clientUpdateProhibited https://icann.org/epp#clientUpdateProhibited
   Name Server: A.IANA-SERVERS.NET
   Name Server: B.IANA-SERVERS.NET
   DNSSEC: signedDelegation
   DNSSEC DS Data: 31589 8 1 3490A6806D47F17A34C29E2CE80E8A999FFBE4BE
   DNSSEC DS Data: 31589 8 2 CDE0D742D6998AA554A92D890F8184C698CFAC8A26FA59875A990C03E576343C
   DNSSEC DS Data: 43547 8 1 B6225AB2CC613E0DCA7962BDC2342EA4F1B56083
   DNSSEC DS Data: 43547 8 2 615A64233543F66F44D68933625B17497C89A70E858ED76A2145997EDF96A918
   DNSSEC DS Data: 31406 8 1 189968811E6EBA862DD6C209F75623D8D9ED9142
   DNSSEC DS Data: 31406 8 2 F78CF3344F72137235098ECBBD08947C2C9001C7F6A085A17F518B5D8F6B916D
   URL of the ICANN Whois Inaccuracy Complaint Form: https://www.icann.org/wicf/
>>> Last update of whois database: 2022-09-16T11:48:14Z <<<

For more information on Whois status codes, please visit https://icann.org/epp

NOTICE: The expiration date displayed in this record is the date the
registrar's sponsorship of the domain name registration in the registry is
currently set to expire. This date does not necessarily reflect the expiration
date of the domain name registrant's agreement with the sponsoring
registrar.  Users may consult the sponsoring registrar's Whois database to
view the registrar's reported date of expiration for this registration.

TERMS OF USE: You are not authorized to access or query our Whois
database through the use of electronic processes that are high-volume and
automated except as reasonably necessary to register domain names or
modify existing registrations; the Data in VeriSign Global Registry
Services' ("VeriSign") Whois database is provided by VeriSign for
information purposes only, and to assist persons in obtaining information
about or related to a domain name registration record. VeriSign does not
guarantee its accuracy. By submitting a Whois query, you agree to abide
by the following terms of use: You agree that you may use this Data only
for lawful purposes and that under no circumstances will you use this Data
to: (1) allow, enable, or otherwise support the transmission of mass
unsolicited, commercial advertising or solicitations via e-mail, telephone,
or facsimile; or (2) enable high volume, automated, electronic processes
that apply to VeriSign (or its computer systems). The compilation,
repackaging, dissemination or other use of this Data is expressly
prohibited without the prior written consent of VeriSign. You agree not to
use electronic processes that are automated and high-volume to access or
query the Whois database except as reasonably necessary to register
domain names or modify existing registrations. VeriSign reserves the right
to restrict your access to the Whois database in its sole discretion to ensure
operational stability.  VeriSign may restrict or terminate your access to the
Whois database for failure to abide by these terms of use. VeriSign
reserves the right to modify these terms at any time.

The Registry database contains ONLY .COM, .NET, .EDU domains and
Registrars.
% IANA WHOIS server
% for more information on IANA, visit http://www.iana.org
% This query returned 1 object

domain:       EXAMPLE.COM

organisation: Internet Assigned Numbers Authority

created:      1992-01-01
source:       IANA
```

- 5. whois コマンド実行(ip アドレス指定)

```
whois 2001:db8::/32
```

```
% [whois.apnic.net]
% Whois data copyright terms    http://www.apnic.net/db/dbcopyright.html

% Information related to '2001:db8::/32'

% Abuse contact for '2001:db8::/32' is 'helpdesk@apnic.net'

inet6num:       2001:db8::/32
netname:        IPV6-DOC-AP
descr:          IPv6 prefix for documentation purpose
country:        AU
org:            ORG-ARAD1-AP
admin-c:        HM20-AP
tech-c:         HM20-AP
status:         ALLOCATED PORTABLE
remarks:        This address range is to be used for documentation
remarks:        purpose only. For more information please see
remarks:        http://www.apnic.net/info/faq/ipv6-documentation-prefix-faq.html
mnt-by:         APNIC-HM
mnt-lower:      APNIC-HM
mnt-routes:     APNIC-HM
mnt-irt:        IRT-APNIC-AP
last-modified:  2017-10-06T12:52:11Z
source:         APNIC

irt:            IRT-APNIC-AP
address:        Brisbane, Australia
e-mail:         helpdesk@apnic.net
abuse-mailbox:  helpdesk@apnic.net
admin-c:        HM20-AP
tech-c:         NO4-AP
auth:           # Filtered
remarks:        APNIC is a Regional Internet Registry.
remarks:        We do not operate the referring network and
remarks:        are unable to investigate complaints of network abuse.
remarks:        For information about IRT, see www.apnic.net/irt
remarks:        helpdesk@apnic.net was validated on 2020-02-03
mnt-by:         APNIC-HM
last-modified:  2020-02-03T02:04:33Z
source:         APNIC

organisation:   ORG-ARAD1-AP
org-name:       APNIC Research and Development
country:        AU
address:        6 Cordelia St
phone:          +61-7-38583100
fax-no:         +61-7-38583199
e-mail:         helpdesk@apnic.net
mnt-ref:        APNIC-HM
mnt-by:         APNIC-HM
last-modified:  2017-10-11T01:28:39Z
source:         APNIC

role:           APNIC Hostmaster
address:        6 Cordelia Street
address:        South Brisbane
address:        QLD 4101
country:        AU
phone:          +61 7 3858 3100
fax-no:         +61 7 3858 3199
e-mail:         helpdesk@apnic.net
admin-c:        AMS11-AP
tech-c:         AH256-AP
nic-hdl:        HM20-AP
remarks:        Administrator for APNIC
notify:         hostmaster@apnic.net
mnt-by:         MAINT-APNIC-AP
last-modified:  2013-10-23T04:06:51Z
source:         APNIC
```
