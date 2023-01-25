; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

; Function Attrs: nofree nosync nounwind readnone
define private fastcc i32 @_f__void__int__faculty__int(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 2
  br i1 %2, label %common.ret, label %if.exit.L2.preheader

if.exit.L2.preheader:                             ; preds = %1
  %3 = tail call i32 @llvm.usub.sat.i32(i32 %0, i32 2)
  %4 = add nuw i32 %3, 1
  %min.iters.check = icmp ult i32 %3, 7
  br i1 %min.iters.check, label %if.exit.L2.preheader7, label %vector.ph

vector.ph:                                        ; preds = %if.exit.L2.preheader
  %n.vec = and i32 %4, -8
  %ind.end = sub i32 %0, %n.vec
  %.splatinsert = insertelement <4 x i32> poison, i32 %0, i64 0
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> poison, <4 x i32> zeroinitializer
  %induction = add <4 x i32> %.splat, <i32 0, i32 -1, i32 -2, i32 -3>
  %5 = add i32 %n.vec, -8
  %6 = lshr exact i32 %5, 3
  %7 = add nuw nsw i32 %6, 1
  %xtraiter = and i32 %7, 3
  %8 = icmp ult i32 %5, 24
  br i1 %8, label %middle.block.unr-lcssa, label %vector.ph.new

vector.ph.new:                                    ; preds = %vector.ph
  %unroll_iter = and i32 %7, 1073741820
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph.new
  %vec.ind = phi <4 x i32> [ %induction, %vector.ph.new ], [ %vec.ind.next.3, %vector.body ]
  %vec.phi = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph.new ], [ %15, %vector.body ]
  %vec.phi6 = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph.new ], [ %16, %vector.body ]
  %niter = phi i32 [ 0, %vector.ph.new ], [ %niter.next.3, %vector.body ]
  %step.add = add <4 x i32> %vec.ind, <i32 -4, i32 -4, i32 -4, i32 -4>
  %9 = mul <4 x i32> %vec.ind, %vec.phi
  %10 = mul <4 x i32> %step.add, %vec.phi6
  %vec.ind.next = add <4 x i32> %vec.ind, <i32 -8, i32 -8, i32 -8, i32 -8>
  %step.add.1 = add <4 x i32> %vec.ind, <i32 -12, i32 -12, i32 -12, i32 -12>
  %11 = mul <4 x i32> %vec.ind.next, %9
  %12 = mul <4 x i32> %step.add.1, %10
  %vec.ind.next.1 = add <4 x i32> %vec.ind, <i32 -16, i32 -16, i32 -16, i32 -16>
  %step.add.2 = add <4 x i32> %vec.ind, <i32 -20, i32 -20, i32 -20, i32 -20>
  %13 = mul <4 x i32> %vec.ind.next.1, %11
  %14 = mul <4 x i32> %step.add.2, %12
  %vec.ind.next.2 = add <4 x i32> %vec.ind, <i32 -24, i32 -24, i32 -24, i32 -24>
  %step.add.3 = add <4 x i32> %vec.ind, <i32 -28, i32 -28, i32 -28, i32 -28>
  %15 = mul <4 x i32> %vec.ind.next.2, %13
  %16 = mul <4 x i32> %step.add.3, %14
  %vec.ind.next.3 = add <4 x i32> %vec.ind, <i32 -32, i32 -32, i32 -32, i32 -32>
  %niter.next.3 = add i32 %niter, 4
  %niter.ncmp.3 = icmp eq i32 %niter.next.3, %unroll_iter
  br i1 %niter.ncmp.3, label %middle.block.unr-lcssa, label %vector.body, !llvm.loop !0

middle.block.unr-lcssa:                           ; preds = %vector.body, %vector.ph
  %.lcssa9.ph = phi <4 x i32> [ undef, %vector.ph ], [ %15, %vector.body ]
  %.lcssa8.ph = phi <4 x i32> [ undef, %vector.ph ], [ %16, %vector.body ]
  %vec.ind.unr = phi <4 x i32> [ %induction, %vector.ph ], [ %vec.ind.next.3, %vector.body ]
  %vec.phi.unr = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %15, %vector.body ]
  %vec.phi6.unr = phi <4 x i32> [ <i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %16, %vector.body ]
  %lcmp.mod.not = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod.not, label %middle.block, label %vector.body.epil

vector.body.epil:                                 ; preds = %middle.block.unr-lcssa, %vector.body.epil
  %vec.ind.epil = phi <4 x i32> [ %vec.ind.next.epil, %vector.body.epil ], [ %vec.ind.unr, %middle.block.unr-lcssa ]
  %vec.phi.epil = phi <4 x i32> [ %17, %vector.body.epil ], [ %vec.phi.unr, %middle.block.unr-lcssa ]
  %vec.phi6.epil = phi <4 x i32> [ %18, %vector.body.epil ], [ %vec.phi6.unr, %middle.block.unr-lcssa ]
  %epil.iter = phi i32 [ %epil.iter.next, %vector.body.epil ], [ 0, %middle.block.unr-lcssa ]
  %step.add.epil = add <4 x i32> %vec.ind.epil, <i32 -4, i32 -4, i32 -4, i32 -4>
  %17 = mul <4 x i32> %vec.ind.epil, %vec.phi.epil
  %18 = mul <4 x i32> %step.add.epil, %vec.phi6.epil
  %vec.ind.next.epil = add <4 x i32> %vec.ind.epil, <i32 -8, i32 -8, i32 -8, i32 -8>
  %epil.iter.next = add i32 %epil.iter, 1
  %epil.iter.cmp.not = icmp eq i32 %epil.iter.next, %xtraiter
  br i1 %epil.iter.cmp.not, label %middle.block, label %vector.body.epil, !llvm.loop !2

middle.block:                                     ; preds = %vector.body.epil, %middle.block.unr-lcssa
  %.lcssa9 = phi <4 x i32> [ %.lcssa9.ph, %middle.block.unr-lcssa ], [ %17, %vector.body.epil ]
  %.lcssa8 = phi <4 x i32> [ %.lcssa8.ph, %middle.block.unr-lcssa ], [ %18, %vector.body.epil ]
  %bin.rdx = mul <4 x i32> %.lcssa8, %.lcssa9
  %19 = tail call i32 @llvm.vector.reduce.mul.v4i32(<4 x i32> %bin.rdx)
  %cmp.n = icmp eq i32 %4, %n.vec
  br i1 %cmp.n, label %common.ret, label %if.exit.L2.preheader7

if.exit.L2.preheader7:                            ; preds = %if.exit.L2.preheader, %middle.block
  %.tr4.ph = phi i32 [ %0, %if.exit.L2.preheader ], [ %ind.end, %middle.block ]
  %accumulator.tr3.ph = phi i32 [ 1, %if.exit.L2.preheader ], [ %19, %middle.block ]
  br label %if.exit.L2

common.ret:                                       ; preds = %if.exit.L2, %middle.block, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %19, %middle.block ], [ %21, %if.exit.L2 ]
  ret i32 %accumulator.tr.lcssa

if.exit.L2:                                       ; preds = %if.exit.L2.preheader7, %if.exit.L2
  %.tr4 = phi i32 [ %20, %if.exit.L2 ], [ %.tr4.ph, %if.exit.L2.preheader7 ]
  %accumulator.tr3 = phi i32 [ %21, %if.exit.L2 ], [ %accumulator.tr3.ph, %if.exit.L2.preheader7 ]
  %20 = add nsw i32 %.tr4, -1
  %21 = mul i32 %.tr4, %accumulator.tr3
  %22 = icmp ult i32 %.tr4, 3
  br i1 %22, label %common.ret, label %if.exit.L2, !llvm.loop !4
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_f__void__int__faculty__int(i32 10) #5
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.0, i32 10, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind readnone willreturn
declare i32 @llvm.vector.reduce.mul.v4i32(<4 x i32>) #3

; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn
declare i32 @llvm.usub.sat.i32(i32, i32) #4

attributes #0 = { nofree nosync nounwind readnone }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nocallback nofree nosync nounwind readnone willreturn }
attributes #4 = { nocallback nofree nosync nounwind readnone speculatable willreturn }
attributes #5 = { nounwind }

!0 = distinct !{!0, !1}
!1 = !{!"llvm.loop.isvectorized", i32 1}
!2 = distinct !{!2, !3}
!3 = !{!"llvm.loop.unroll.disable"}
!4 = distinct !{!4, !5, !1}
!5 = !{!"llvm.loop.unroll.runtime.disable"}
