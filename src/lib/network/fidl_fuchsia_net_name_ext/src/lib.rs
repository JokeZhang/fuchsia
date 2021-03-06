// Copyright 2020 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//! Extensions for types in the `fidl_fuchsia_net_name` crate.

#![deny(missing_docs, unreachable_patterns)]

use fidl_fuchsia_net_name as fname;

/// A manual implementation of `From`.
pub trait FromExt<T> {
    /// Performs the conversion.
    fn from_ext(f: T) -> Self;
}

/// A manual implementation of `Into`.
///
/// A blanket implementation is provided for implementers of `FromExt<T>`.
pub trait IntoExt<T> {
    /// Performs the conversion.
    fn into_ext(self) -> T;
}

impl<T, U> IntoExt<U> for T
where
    U: FromExt<T>,
{
    fn into_ext(self) -> U {
        U::from_ext(self)
    }
}

impl FromExt<fname::StaticDnsServerSource> for fname::DnsServerSource {
    fn from_ext(f: fname::StaticDnsServerSource) -> fname::DnsServerSource {
        fname::DnsServerSource::StaticSource(f)
    }
}

impl FromExt<fname::DhcpDnsServerSource> for fname::DnsServerSource {
    fn from_ext(f: fname::DhcpDnsServerSource) -> fname::DnsServerSource {
        fname::DnsServerSource::Dhcp(f)
    }
}

impl FromExt<fname::NdpDnsServerSource> for fname::DnsServerSource {
    fn from_ext(f: fname::NdpDnsServerSource) -> fname::DnsServerSource {
        fname::DnsServerSource::Ndp(f)
    }
}

impl FromExt<fname::Dhcpv6DnsServerSource> for fname::DnsServerSource {
    fn from_ext(f: fname::Dhcpv6DnsServerSource) -> fname::DnsServerSource {
        fname::DnsServerSource::Dhcpv6(f)
    }
}

/// Extension trait that provides a manual implementation of `Clone`.
pub trait CloneExt {
    /// Returns a copy of the value.
    fn clone(&self) -> Self;
}

impl CloneExt for fname::StaticDnsServerSource {
    fn clone(&self) -> fname::StaticDnsServerSource {
        fname::StaticDnsServerSource {}
    }
}

impl CloneExt for fname::DhcpDnsServerSource {
    fn clone(&self) -> fname::DhcpDnsServerSource {
        fname::DhcpDnsServerSource { source_interface: self.source_interface }
    }
}

impl CloneExt for fname::NdpDnsServerSource {
    fn clone(&self) -> fname::NdpDnsServerSource {
        fname::NdpDnsServerSource { source_interface: self.source_interface }
    }
}

impl CloneExt for fname::Dhcpv6DnsServerSource {
    fn clone(&self) -> fname::Dhcpv6DnsServerSource {
        fname::Dhcpv6DnsServerSource { source_interface: self.source_interface }
    }
}

impl CloneExt for fname::DnsServerSource {
    fn clone(&self) -> fname::DnsServerSource {
        match self {
            fname::DnsServerSource::StaticSource(s) => {
                fname::DnsServerSource::StaticSource(s.clone())
            }
            fname::DnsServerSource::Dhcp(s) => fname::DnsServerSource::Dhcp(s.clone()),
            fname::DnsServerSource::Ndp(s) => fname::DnsServerSource::Ndp(s.clone()),
            fname::DnsServerSource::Dhcpv6(s) => fname::DnsServerSource::Dhcpv6(s.clone()),
        }
    }
}

impl CloneExt for fname::DnsServer_ {
    fn clone(&self) -> fname::DnsServer_ {
        fname::DnsServer_ {
            address: self.address.clone(),
            source: self.source.as_ref().map(|x| x.clone()),
        }
    }
}

impl<T: CloneExt> CloneExt for Vec<T> {
    fn clone(&self) -> Self {
        self.iter().map(CloneExt::clone).collect()
    }
}

#[cfg(test)]
mod tests {
    use fidl_fuchsia_net as fnet;

    use super::*;

    #[test]
    fn test_from_into_ext() {
        let a = fname::StaticDnsServerSource {};
        assert_eq!(fname::DnsServerSource::StaticSource(a.clone()), a.into_ext());

        let a = fname::DhcpDnsServerSource { source_interface: Some(1) };
        assert_eq!(fname::DnsServerSource::Dhcp(a.clone()), a.into_ext());

        let a = fname::NdpDnsServerSource { source_interface: Some(1) };
        assert_eq!(fname::DnsServerSource::Ndp(a.clone()), a.into_ext());

        let a = fname::Dhcpv6DnsServerSource { source_interface: Some(1) };
        assert_eq!(fname::DnsServerSource::Dhcpv6(a.clone()), a.into_ext());
    }

    #[test]
    fn test_clone() {
        let a = fname::StaticDnsServerSource {};
        assert_eq!(a.clone(), a);

        let a = fname::DhcpDnsServerSource { source_interface: Some(1) };
        assert_eq!(a.clone(), a);

        let a = fname::NdpDnsServerSource { source_interface: Some(1) };
        assert_eq!(a.clone(), a);

        let a = fname::Dhcpv6DnsServerSource { source_interface: Some(1) };
        assert_eq!(a.clone(), a);

        let a =
            fname::DnsServerSource::Dhcp(fname::DhcpDnsServerSource { source_interface: Some(1) });
        assert_eq!(a.clone(), a);

        let a = fname::DnsServer_ {
            address: Some(fnet::SocketAddress::Ipv4(fnet::Ipv4SocketAddress {
                address: fnet::Ipv4Address { addr: [8, 8, 4, 4] },
                port: 53,
            })),
            source: Some(fname::DnsServerSource::Dhcp(fname::DhcpDnsServerSource {
                source_interface: Some(1),
            })),
        };
        assert_eq!(a.clone(), a);
    }
}
