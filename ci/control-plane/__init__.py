"""Phase 0 linearizable control-plane foundation for BFME2-Rebuild.

Implements the Role-Checked Authority Mediator, transactional DynamoDB-compatible
Authority Store contract (single-Update(authority_state) invariant + ALL_OLD
multi-item CancellationReasons precedence), write-ahead ruleset mutation journal,
and executable CI workflow lints (a)-(j).
"""
