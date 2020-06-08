// Copyright 2019 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

use fidl_fuchsia_overnet_protocol::TRANSFER_KEY_LENGTH;
use rand::Rng;
use std::array::TryFromSliceError;
use std::convert::{TryFrom, TryInto};

/// Labels the endpoint of a client/server connection.
#[derive(Debug, PartialEq, Eq, PartialOrd, Ord, Clone, Copy)]
pub enum Endpoint {
    /// Client endpoint.
    Client,
    /// Server endpoint.
    Server,
}

impl Endpoint {
    pub(crate) fn quic_id_bit(&self) -> u64 {
        match self {
            Endpoint::Client => 0,
            Endpoint::Server => 1,
        }
    }

    /// Returns the other end of this endpoint.
    pub fn opposite(&self) -> Endpoint {
        match self {
            Endpoint::Client => Endpoint::Server,
            Endpoint::Server => Endpoint::Client,
        }
    }
}

/// Labels a node with a mesh-unique address
#[derive(PartialEq, PartialOrd, Eq, Ord, Clone, Copy, Hash, Debug)]
pub struct NodeId(pub u64);

impl From<u64> for NodeId {
    fn from(id: u64) -> Self {
        NodeId(id)
    }
}

impl From<NodeId> for fidl_fuchsia_overnet_protocol::NodeId {
    fn from(id: NodeId) -> Self {
        Self { id: id.0 }
    }
}

impl From<&NodeId> for fidl_fuchsia_overnet_protocol::NodeId {
    fn from(id: &NodeId) -> Self {
        Self { id: id.0 }
    }
}

impl From<fidl_fuchsia_overnet_protocol::NodeId> for NodeId {
    fn from(id: fidl_fuchsia_overnet_protocol::NodeId) -> Self {
        id.id.into()
    }
}

/// Labels a link with a node-unique identifier
#[derive(PartialEq, PartialOrd, Eq, Ord, Clone, Copy, Debug, Hash)]
pub struct NodeLinkId(pub u64);

impl From<u64> for NodeLinkId {
    fn from(id: u64) -> Self {
        NodeLinkId(id)
    }
}

pub(crate) type TransferKey = [u8; TRANSFER_KEY_LENGTH as usize];

pub(crate) fn generate_transfer_key() -> TransferKey {
    rand::thread_rng().gen::<TransferKey>()
}

/// Labels a quic connection
#[derive(PartialEq, PartialOrd, Eq, Ord, Clone, Copy, Hash)]
pub struct ConnectionId([u8; quiche::MAX_CONN_ID_LEN]);

impl ConnectionId {
    pub(crate) fn new() -> Self {
        ConnectionId(rand::thread_rng().gen())
    }

    fn from_slice(slice: &[u8]) -> Result<Self, TryFromSliceError> {
        Ok(ConnectionId(slice.try_into()?))
    }

    pub(crate) fn to_array(&self) -> [u8; quiche::MAX_CONN_ID_LEN] {
        self.0
    }
}

impl std::fmt::Debug for ConnectionId {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_str("ConnectionId(")?;
        base64::encode(&self.0).fmt(f)?;
        f.write_str(")")
    }
}

impl TryFrom<&[u8]> for ConnectionId {
    type Error = TryFromSliceError;
    fn try_from(slice: &[u8]) -> Result<Self, Self::Error> {
        ConnectionId::from_slice(slice)
    }
}

impl TryFrom<&Vec<u8>> for ConnectionId {
    type Error = TryFromSliceError;
    fn try_from(vec: &Vec<u8>) -> Result<Self, Self::Error> {
        ConnectionId::from_slice(vec.as_slice())
    }
}

impl TryFrom<Vec<u8>> for ConnectionId {
    type Error = TryFromSliceError;
    fn try_from(vec: Vec<u8>) -> Result<Self, Self::Error> {
        ConnectionId::from_slice(vec.as_slice())
    }
}
