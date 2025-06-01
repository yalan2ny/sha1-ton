/*
    This file is part of TON Blockchain Library.

    TON Blockchain Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    TON Blockchain Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TON Blockchain Library.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2017-2020 Telegram Systems LLP
*/
#pragma once

#include "auto/tl/ton_api.h"
#include "td/utils/buffer.h"
#include "td/utils/int_types.h"

#include "td/actor/actor.h"

#include "adnl/adnl.h"

#include "overlay-manager.h"

namespace ton {

namespace overlay {

class Overlay : public td::actor::Actor {
 public:
  using BroadcastHash = td::Bits256;
  using BroadcastDataHash = td::Bits256;
  using BroadcastPartHash = td::Bits256;

  static td::actor::ActorOwn<Overlay> create_public(
      td::actor::ActorId<keyring::Keyring> keyring, td::actor::ActorId<adnl::Adnl> adnl,
      td::actor::ActorId<OverlayManager> manager, td::actor::ActorId<dht::Dht> dht_node, adnl::AdnlNodeIdShort local_id,
      OverlayIdFull overlay_id, std::unique_ptr<Overlays::Callback> callback, OverlayPrivacyRules rules,
      td::string scope, OverlayOptions opts = {});
  static td::actor::ActorOwn<Overlay> create_private(
      td::actor::ActorId<keyring::Keyring> keyring, td::actor::ActorId<adnl::Adnl> adnl,
      td::actor::ActorId<OverlayManager> manager, td::actor::ActorId<dht::Dht> dht_node, adnl::AdnlNodeIdShort local_id,
      OverlayIdFull overlay_id, std::vector<adnl::AdnlNodeIdShort> nodes, std::unique_ptr<Overlays::Callback> callback,
      OverlayPrivacyRules rules, std::string scope, OverlayOptions opts = {});
  static td::actor::ActorOwn<Overlay> create_semiprivate(
      td::actor::ActorId<keyring::Keyring> keyring, td::actor::ActorId<adnl::Adnl> adnl,
      td::actor::ActorId<OverlayManager> manager, td::actor::ActorId<dht::Dht> dht_node, adnl::AdnlNodeIdShort local_id,
      OverlayIdFull overlay_id, std::vector<adnl::AdnlNodeIdShort> nodes, std::vector<PublicKeyHash> root_public_keys,
      OverlayMemberCertificate cert, std::unique_ptr<Overlays::Callback> callback, OverlayPrivacyRules rules,
      std::string scope, OverlayOptions opts = {});

  virtual void update_dht_node(td::actor::ActorId<dht::Dht> dht) = 0;

  virtual void receive_message(adnl::AdnlNodeIdShort src, tl_object_ptr<ton_api::overlay_messageExtra> extra,
                               td::BufferSlice data) = 0;
  virtual void receive_query(adnl::AdnlNodeIdShort src, tl_object_ptr<ton_api::overlay_messageExtra> extra,
                             td::BufferSlice data, td::Promise<td::BufferSlice> promise) = 0;
  virtual void send_message_to_neighbours(td::BufferSlice data) = 0;
  virtual void send_broadcast(PublicKeyHash send_as, td::uint32 flags, td::BufferSlice data) = 0;
  virtual void send_broadcast_fec(PublicKeyHash send_as, td::uint32 flags, td::BufferSlice data) = 0;
  virtual void print(td::StringBuilder &sb) = 0;
  virtual void get_overlay_random_peers(td::uint32 max_peers,
                                        td::Promise<std::vector<adnl::AdnlNodeIdShort>> promise) = 0;
  virtual void add_certificate(PublicKeyHash key, std::shared_ptr<Certificate>) = 0;
  virtual void set_privacy_rules(OverlayPrivacyRules rules) = 0;
  virtual void receive_nodes_from_db(tl_object_ptr<ton_api::overlay_nodes> nodes) = 0;
  virtual void receive_nodes_from_db_v2(tl_object_ptr<ton_api::overlay_nodesV2> nodes) = 0;
  virtual void get_stats(td::Promise<tl_object_ptr<ton_api::engine_validator_overlayStats>> promise) = 0;
  virtual void update_throughput_out_ctr(adnl::AdnlNodeIdShort peer_id, td::uint64 msg_size, bool is_query,
                                         bool is_response) = 0;
  virtual void update_throughput_in_ctr(adnl::AdnlNodeIdShort peer_id, td::uint64 msg_size, bool is_query,
                                        bool is_response) = 0;
  virtual void update_peer_ip_str(adnl::AdnlNodeIdShort peer_id, td::string ip_str) = 0;
  virtual void update_member_certificate(OverlayMemberCertificate cert) = 0;
  virtual void update_root_member_list(std::vector<adnl::AdnlNodeIdShort> ids,
                                       std::vector<PublicKeyHash> root_public_keys, OverlayMemberCertificate cert) = 0;
  //virtual void receive_broadcast(td::BufferSlice data) = 0;
  //virtual void subscribe(std::unique_ptr<Overlays::Callback> callback) = 0;
  virtual void forget_peer(adnl::AdnlNodeIdShort peer_id) = 0;
};

}  // namespace overlay

}  // namespace ton
