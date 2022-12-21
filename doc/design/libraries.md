# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libwaltercoin_cli*         | RPC client functionality used by *waltercoin-cli* executable |
| *libwaltercoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libwaltercoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libwaltercoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libwaltercoin_node* and *libwaltercoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libwaltercoinconsensus*    | Shared library build of static *libwaltercoin_consensus* library |
| *libwaltercoin_kernel*      | Consensus engine and support library used for validation by *libwaltercoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libwaltercoinqt*           | GUI functionality used by *waltercoin-qt* and *waltercoin-gui* executables |
| *libwaltercoin_ipc*         | IPC functionality used by *waltercoin-node*, *waltercoin-wallet*, *waltercoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libwaltercoin_node*        | P2P and RPC server functionality used by *waltercoind* and *waltercoin-qt* executables. |
| *libwaltercoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libwaltercoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libwaltercoin_wallet*      | Wallet functionality used by *waltercoind* and *waltercoin-wallet* executables. |
| *libwaltercoin_wallet_tool* | Lower-level wallet functionality used by *waltercoin-wallet* executable. |
| *libwaltercoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *waltercoind* and *waltercoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libwaltercoin_consensus* and *libwaltercoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libwaltercoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libwaltercoin_node* code lives in `src/node/` in the `node::` namespace
  - *libwaltercoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libwaltercoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libwaltercoin_util* code lives in `src/util/` in the `util::` namespace
  - *libwaltercoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "linear" }}}%%

graph TD;

waltercoin-cli[waltercoin-cli]-->libwaltercoin_cli;

waltercoind[waltercoind]-->libwaltercoin_node;
waltercoind[waltercoind]-->libwaltercoin_wallet;

waltercoin-qt[waltercoin-qt]-->libwaltercoin_node;
waltercoin-qt[waltercoin-qt]-->libwaltercoinqt;
waltercoin-qt[waltercoin-qt]-->libwaltercoin_wallet;

waltercoin-wallet[waltercoin-wallet]-->libwaltercoin_wallet;
waltercoin-wallet[waltercoin-wallet]-->libwaltercoin_wallet_tool;

libwaltercoin_cli-->libwaltercoin_common;
libwaltercoin_cli-->libwaltercoin_util;

libwaltercoin_common-->libwaltercoin_util;
libwaltercoin_common-->libwaltercoin_consensus;

libwaltercoin_kernel-->libwaltercoin_consensus;
libwaltercoin_kernel-->libwaltercoin_util;

libwaltercoin_node-->libwaltercoin_common;
libwaltercoin_node-->libwaltercoin_consensus;
libwaltercoin_node-->libwaltercoin_kernel;
libwaltercoin_node-->libwaltercoin_util;

libwaltercoinqt-->libwaltercoin_common;
libwaltercoinqt-->libwaltercoin_util;

libwaltercoin_wallet-->libwaltercoin_common;
libwaltercoin_wallet-->libwaltercoin_util;

libwaltercoin_wallet_tool-->libwaltercoin_util;
libwaltercoin_wallet_tool-->libwaltercoin_wallet;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class waltercoin-qt,waltercoind,waltercoin-cli,waltercoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libwaltercoin_wallet* and *libwaltercoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code still is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libwaltercoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libwaltercoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libwaltercoin_common* should serve a similar function as *libwaltercoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libwaltercoin_util* and *libwaltercoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for waltercoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libwaltercoin_kernel* is not supposed to depend on *libwaltercoin_common*, only *libwaltercoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libwaltercoin_kernel* should only depend on *libwaltercoin_util* and *libwaltercoin_consensus*.

- The only thing that should depend on *libwaltercoin_kernel* internally should be *libwaltercoin_node*. GUI and wallet libraries *libwaltercoinqt* and *libwaltercoin_wallet* in particular should not depend on *libwaltercoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libwaltercoin_consensus*, *libwaltercoin_common*, and *libwaltercoin_util*, instead of *libwaltercoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libwaltercoinqt*, *libwaltercoin_node*, *libwaltercoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libwaltercoin_node* to *libwaltercoin_kernel* as part of [The libwaltercoinkernel Project #24303](https://github.com/waltercoin/waltercoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/waltercoin/waltercoin/issues/15732)
