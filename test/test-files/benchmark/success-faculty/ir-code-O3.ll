; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 4

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define private fastcc i32 @_Z7facultyi(i32 range(i32 1, 11) %0) unnamed_addr #0 {
  %2 = icmp samesign ult i32 %0, 2
  br i1 %2, label %common.ret, label %vector.ph

vector.ph:                                        ; preds = %1
  %n.rnd.up = add nuw nsw i32 %0, 2
  %n.vec = and i32 %n.rnd.up, 28
  %trip.count.minus.1 = add nsw i32 %0, -2
  %broadcast.splatinsert = insertelement <4 x i32> poison, i32 %trip.count.minus.1, i64 0
  %broadcast.splat = shufflevector <4 x i32> %broadcast.splatinsert, <4 x i32> poison, <4 x i32> zeroinitializer
  %broadcast.splatinsert5 = insertelement <4 x i32> poison, i32 %0, i64 0
  %broadcast.splat6 = shufflevector <4 x i32> %broadcast.splatinsert5, <4 x i32> poison, <4 x i32> zeroinitializer
  %induction = add nsw <4 x i32> %broadcast.splat6, <i32 0, i32 -1, i32 -2, i32 -3>
  %3 = icmp eq i32 %n.vec, 4
  br i1 %3, label %middle.block, label %vector.body.1, !llvm.loop !5

vector.body.1:                                    ; preds = %vector.ph
  %vec.ind.next = add nsw <4 x i32> %broadcast.splat6, <i32 -4, i32 -5, i32 -6, i32 -7>
  %4 = mul nsw <4 x i32> %vec.ind.next, %induction
  %5 = icmp eq i32 %n.vec, 8
  br i1 %5, label %middle.block, label %vector.body.2, !llvm.loop !5

vector.body.2:                                    ; preds = %vector.body.1
  %vec.ind.next.1 = add nsw <4 x i32> %broadcast.splat6, <i32 -8, i32 -9, i32 -10, i32 -11>
  %6 = mul nsw <4 x i32> %vec.ind.next.1, %4
  br label %middle.block

middle.block:                                     ; preds = %vector.body.2, %vector.body.1, %vector.ph
  %index.lcssa = phi i32 [ 0, %vector.ph ], [ 4, %vector.body.1 ], [ 8, %vector.body.2 ]
  %vec.phi.lcssa = phi <4 x i32> [ splat (i32 1), %vector.ph ], [ %induction, %vector.body.1 ], [ %4, %vector.body.2 ]
  %.lcssa = phi <4 x i32> [ %induction, %vector.ph ], [ %4, %vector.body.1 ], [ %6, %vector.body.2 ]
  %broadcast.splatinsert7 = insertelement <4 x i32> poison, i32 %index.lcssa, i64 0
  %broadcast.splat8 = shufflevector <4 x i32> %broadcast.splatinsert7, <4 x i32> poison, <4 x i32> zeroinitializer
  %vec.iv = or disjoint <4 x i32> %broadcast.splat8, <i32 0, i32 1, i32 2, i32 3>
  %.not = icmp ugt <4 x i32> %vec.iv, %broadcast.splat
  %7 = select <4 x i1> %.not, <4 x i32> %vec.phi.lcssa, <4 x i32> %.lcssa
  %8 = tail call i32 @llvm.vector.reduce.mul.v4i32(<4 x i32> %7)
  br label %common.ret

common.ret:                                       ; preds = %middle.block, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %8, %middle.block ]
  ret i32 %accumulator.tr.lcssa
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_Z7facultyi(i32 10) #4
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 10, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.mul.v4i32(<4 x i32>) #3

attributes #0 = { nofree norecurse nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = distinct !{!5, !6, !7}
!6 = !{!"llvm.loop.isvectorized", i32 1}
!7 = !{!"llvm.loop.unroll.runtime.disable"}
