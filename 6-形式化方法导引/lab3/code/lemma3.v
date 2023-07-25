Lemma ex3 : forall T (P : T -> Prop),
    (~ exists x, P x) -> forall x, ~ P x.
Proof.
  intros T P H x P_x.
  apply H.
  exists x.
  exact P_x.
Qed.