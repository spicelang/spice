; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nosync nounwind readnone
define i32 @"faculty(int)"(i32 %0) local_unnamed_addr #1 {
entry:
  %1 = icmp slt i32 %0, 2
  br i1 %1, label %common.ret, label %if.end.preheader

if.end.preheader:                                 ; preds = %entry
  %2 = add i32 %0, -1
  %min.iters.check = icmp ult i32 %2, 8
  br i1 %min.iters.check, label %if.end.preheader12, label %vector.ph

vector.ph:                                        ; preds = %if.end.preheader
  %n.vec = and i32 %2, -8
  %ind.end = sub i32 %0, %n.vec
  %.splatinsert = insertelement <4 x i32> poison, i32 %0, i32 0
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> poison, <4 x i32> zeroinitializer
  %induction = add <4 x i32> %.splat, <i32 0, i32 -1, i32 -2, i32 -3>
  %3 = add i32 %n.vec, -8
  %4 = lshr exact i32 %3, 3
  %5 = add nuw nsw i32 %4, 1
  %xtraiter = and i32 %5, 3
  %6 = icmp ult i32 %3, 24
  br i1 %6, label %middle.block.unr-lcssa, label %vector.ph.new

vector.ph.new:                                    ; preds = %vector.ph
  %unroll_iter = and i32 %5, 1073741820
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph.new
  %vec.ind = phi <4 x i32> [ %induction, %vector.ph.new ], [ %vec.ind.next.3, %vector.body ]
  %vec.phi = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph.new ], [ %13, %vector.body ]
  %vec.phi11 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph.new ], [ %14, %vector.body ]
  %niter = phi i32 [ %unroll_iter, %vector.ph.new ], [ %niter.nsub.3, %vector.body ]
  %step.add = add <4 x i32> %vec.ind, <i32 -4, i32 -4, i32 -4, i32 -4>
  %7 = mul <4 x i32> %vec.ind, %vec.phi
  %8 = mul <4 x i32> %step.add, %vec.phi11
  %vec.ind.next = add <4 x i32> %vec.ind, <i32 -8, i32 -8, i32 -8, i32 -8>
  %step.add.1 = add <4 x i32> %vec.ind, <i32 -12, i32 -12, i32 -12, i32 -12>
  %9 = mul <4 x i32> %vec.ind.next, %7
  %10 = mul <4 x i32> %step.add.1, %8
  %vec.ind.next.1 = add <4 x i32> %vec.ind, <i32 -16, i32 -16, i32 -16, i32 -16>
  %step.add.2 = add <4 x i32> %vec.ind, <i32 -20, i32 -20, i32 -20, i32 -20>
  %11 = mul <4 x i32> %vec.ind.next.1, %9
  %12 = mul <4 x i32> %step.add.2, %10
  %vec.ind.next.2 = add <4 x i32> %vec.ind, <i32 -24, i32 -24, i32 -24, i32 -24>
  %step.add.3 = add <4 x i32> %vec.ind, <i32 -28, i32 -28, i32 -28, i32 -28>
  %13 = mul <4 x i32> %vec.ind.next.2, %11
  %14 = mul <4 x i32> %step.add.3, %12
  %vec.ind.next.3 = add <4 x i32> %vec.ind, <i32 -32, i32 -32, i32 -32, i32 -32>
  %niter.nsub.3 = add i32 %niter, -4
  %niter.ncmp.3 = icmp eq i32 %niter.nsub.3, 0
  br i1 %niter.ncmp.3, label %middle.block.unr-lcssa, label %vector.body, !llvm.loop !0

middle.block.unr-lcssa:                           ; preds = %vector.body, %vector.ph
  %.lcssa14.ph = phi <4 x i32> [ undef, %vector.ph ], [ %13, %vector.body ]
  %.lcssa13.ph = phi <4 x i32> [ undef, %vector.ph ], [ %14, %vector.body ]
  %vec.ind.unr = phi <4 x i32> [ %induction, %vector.ph ], [ %vec.ind.next.3, %vector.body ]
  %vec.phi.unr = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %13, %vector.body ]
  %vec.phi11.unr = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %14, %vector.body ]
  %lcmp.mod.not = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod.not, label %middle.block, label %vector.body.epil

vector.body.epil:                                 ; preds = %middle.block.unr-lcssa, %vector.body.epil
  %vec.ind.epil = phi <4 x i32> [ %vec.ind.next.epil, %vector.body.epil ], [ %vec.ind.unr, %middle.block.unr-lcssa ]
  %vec.phi.epil = phi <4 x i32> [ %15, %vector.body.epil ], [ %vec.phi.unr, %middle.block.unr-lcssa ]
  %vec.phi11.epil = phi <4 x i32> [ %16, %vector.body.epil ], [ %vec.phi11.unr, %middle.block.unr-lcssa ]
  %epil.iter = phi i32 [ %epil.iter.sub, %vector.body.epil ], [ %xtraiter, %middle.block.unr-lcssa ]
  %step.add.epil = add <4 x i32> %vec.ind.epil, <i32 -4, i32 -4, i32 -4, i32 -4>
  %15 = mul <4 x i32> %vec.ind.epil, %vec.phi.epil
  %16 = mul <4 x i32> %step.add.epil, %vec.phi11.epil
  %vec.ind.next.epil = add <4 x i32> %vec.ind.epil, <i32 -8, i32 -8, i32 -8, i32 -8>
  %epil.iter.sub = add i32 %epil.iter, -1
  %epil.iter.cmp.not = icmp eq i32 %epil.iter.sub, 0
  br i1 %epil.iter.cmp.not, label %middle.block, label %vector.body.epil, !llvm.loop !2

middle.block:                                     ; preds = %vector.body.epil, %middle.block.unr-lcssa
  %.lcssa14 = phi <4 x i32> [ %.lcssa14.ph, %middle.block.unr-lcssa ], [ %15, %vector.body.epil ]
  %.lcssa13 = phi <4 x i32> [ %.lcssa13.ph, %middle.block.unr-lcssa ], [ %16, %vector.body.epil ]
  %bin.rdx = mul <4 x i32> %.lcssa13, %.lcssa14
  %17 = call i32 @llvm.vector.reduce.mul.v4i32(<4 x i32> %bin.rdx)
  %cmp.n = icmp eq i32 %2, %n.vec
  br i1 %cmp.n, label %common.ret, label %if.end.preheader12

if.end.preheader12:                               ; preds = %if.end.preheader, %middle.block
  %.tr9.ph = phi i32 [ %0, %if.end.preheader ], [ %ind.end, %middle.block ]
  %accumulator.tr8.ph = phi i32 [ 1, %if.end.preheader ], [ %17, %middle.block ]
  br label %if.end

common.ret:                                       ; preds = %if.end, %middle.block, %entry
  %accumulator.tr.lcssa = phi i32 [ 1, %entry ], [ %17, %middle.block ], [ %19, %if.end ]
  ret i32 %accumulator.tr.lcssa

if.end:                                           ; preds = %if.end.preheader12, %if.end
  %.tr9 = phi i32 [ %18, %if.end ], [ %.tr9.ph, %if.end.preheader12 ]
  %accumulator.tr8 = phi i32 [ %19, %if.end ], [ %accumulator.tr8.ph, %if.end.preheader12 ]
  %18 = add nsw i32 %.tr9, -1
  %19 = mul i32 %.tr9, %accumulator.tr8
  %20 = icmp slt i32 %.tr9, 3
  br i1 %20, label %common.ret, label %if.end, !llvm.loop !4
}

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry:
  %0 = tail call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([21 x i8], [21 x i8]* @0, i64 0, i64 0), i32 10, i32 3628800)
  ret i32 0
}

; Function Attrs: nofree nosync nounwind readnone willreturn
declare i32 @llvm.vector.reduce.mul.v4i32(<4 x i32>) #2

attributes #0 = { nofree nounwind }
attributes #1 = { nofree nosync nounwind readnone }
attributes #2 = { nofree nosync nounwind readnone willreturn }

!0 = distinct !{!0, !1}
!1 = !{!"llvm.loop.isvectorized", i32 1}
!2 = distinct !{!2, !3}
!3 = !{!"llvm.loop.unroll.disable"}
!4 = distinct !{!4, !5, !1}
!5 = !{!"llvm.loop.unroll.runtime.disable"}
