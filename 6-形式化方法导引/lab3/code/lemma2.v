Lemma ex2 : forall A B, (A \/ B) -> ~(~A /\ ~B).
Proof.
  intros A B A_or_B.
  unfold not.
  intros [not_A not_B].
  destruct A_or_B as [A_true | B_true].
  - apply not_A in A_true. contradiction.
  - apply not_B in B_true. contradiction.
Qed.