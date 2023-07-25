Lemma ex1 : forall A : Prop, ~~~A -> ~A.
Proof.
  intros A H1 H2.
  apply H1.
  intros H3.
  apply H3.
  apply H2.
Qed.