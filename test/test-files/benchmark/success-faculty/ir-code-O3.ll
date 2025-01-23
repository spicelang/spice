; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define private fastcc i32 @_Z7facultyi(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 2
  br i1 %2, label %common.ret, label %if.exit.L2.preheader

if.exit.L2.preheader:                             ; preds = %1
  %3 = add nsw i32 %0, -1
  %min.iters.check = icmp ult i32 %0, 9
  br i1 %min.iters.check, label %if.exit.L2.preheader7, label %vector.ph

if.exit.L2.preheader7:                            ; preds = %middle.block, %if.exit.L2.preheader
  %.tr4.ph = phi i32 [ %0, %if.exit.L2.preheader ], [ %ind.end, %middle.block ]
  %accumulator.tr3.ph = phi i32 [ 1, %if.exit.L2.preheader ], [ %7, %middle.block ]
  br label %if.exit.L2

vector.ph:                                        ; preds = %if.exit.L2.preheader
  %n.vec = and i32 %3, -8
  %ind.end = sub i32 %0, %n.vec
  %.splatinsert = insertelement <4 x i32> poison, i32 %0, i64 0
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> poison, <4 x i32> zeroinitializer
  %induction = add nsw <4 x i32> %.splat, <i32 0, i32 -1, i32 -2, i32 -3>
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i32 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %vec.ind = phi <4 x i32> [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %4, %vector.body ]
  %vec.phi6 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %5, %vector.body ]
  %step.add = add <4 x i32> %vec.ind, <i32 -4, i32 -4, i32 -4, i32 -4>
  %4 = mul <4 x i32> %vec.ind, %vec.phi
  %5 = mul <4 x i32> %step.add, %vec.phi6
  %index.next = add nuw i32 %index, 8
  %vec.ind.next = add <4 x i32> %vec.ind, <i32 -8, i32 -8, i32 -8, i32 -8>
  %6 = icmp eq i32 %index.next, %n.vec
  br i1 %6, label %middle.block, label %vector.body, !llvm.loop !5

middle.block:                                     ; preds = %vector.body
  %bin.rdx = mul <4 x i32> %5, %4
  %7 = tail call i32 @llvm.vector.reduce.mul.v4i32(<4 x i32> %bin.rdx)
  %cmp.n = icmp eq i32 %3, %n.vec
  br i1 %cmp.n, label %common.ret, label %if.exit.L2.preheader7

common.ret:                                       ; preds = %if.exit.L2, %middle.block, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %7, %middle.block ], [ %9, %if.exit.L2 ]
  ret i32 %accumulator.tr.lcssa

if.exit.L2:                                       ; preds = %if.exit.L2.preheader7, %if.exit.L2
  %.tr4 = phi i32 [ %8, %if.exit.L2 ], [ %.tr4.ph, %if.exit.L2.preheader7 ]
  %accumulator.tr3 = phi i32 [ %9, %if.exit.L2 ], [ %accumulator.tr3.ph, %if.exit.L2.preheader7 ]
  %8 = add nsw i32 %.tr4, -1
  %9 = mul nsw i32 %.tr4, %accumulator.tr3
  %10 = icmp ult i32 %.tr4, 3
  br i1 %10, label %common.ret, label %if.exit.L2, !llvm.loop !8
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_Z7facultyi(i32 10) #4
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 10, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

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
!8 = distinct !{!8, !7, !6}
