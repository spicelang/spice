; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 1

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc i32 @_f__void__int__faculty__int(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 2
  br i1 %2, label %common.ret, label %if.exit.L2.preheader

if.exit.L2.preheader:                             ; preds = %1
  %3 = tail call i32 @llvm.usub.sat.i32(i32 %0, i32 2)
  %4 = add nuw i32 %3, 1
  %min.iters.check = icmp ult i32 %3, 31
  br i1 %min.iters.check, label %if.exit.L2.preheader13, label %vector.ph

vector.ph:                                        ; preds = %if.exit.L2.preheader
  %n.vec = and i32 %4, -32
  %ind.end = sub i32 %0, %n.vec
  %.splatinsert = insertelement <8 x i32> poison, i32 %0, i64 0
  %.splat = shufflevector <8 x i32> %.splatinsert, <8 x i32> poison, <8 x i32> zeroinitializer
  %induction = add <8 x i32> %.splat, <i32 0, i32 -1, i32 -2, i32 -3, i32 -4, i32 -5, i32 -6, i32 -7>
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %vector.ph
  %index = phi i32 [ 0, %vector.ph ], [ %index.next, %vector.body ]
  %vec.ind = phi <8 x i32> [ %induction, %vector.ph ], [ %vec.ind.next, %vector.body ]
  %vec.phi = phi <8 x i32> [ <i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %5, %vector.body ]
  %vec.phi8 = phi <8 x i32> [ <i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %6, %vector.body ]
  %vec.phi9 = phi <8 x i32> [ <i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %7, %vector.body ]
  %vec.phi10 = phi <8 x i32> [ <i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1>, %vector.ph ], [ %8, %vector.body ]
  %step.add = add <8 x i32> %vec.ind, <i32 -8, i32 -8, i32 -8, i32 -8, i32 -8, i32 -8, i32 -8, i32 -8>
  %step.add5 = add <8 x i32> %vec.ind, <i32 -16, i32 -16, i32 -16, i32 -16, i32 -16, i32 -16, i32 -16, i32 -16>
  %step.add6 = add <8 x i32> %vec.ind, <i32 -24, i32 -24, i32 -24, i32 -24, i32 -24, i32 -24, i32 -24, i32 -24>
  %5 = mul <8 x i32> %vec.ind, %vec.phi
  %6 = mul <8 x i32> %step.add, %vec.phi8
  %7 = mul <8 x i32> %step.add5, %vec.phi9
  %8 = mul <8 x i32> %step.add6, %vec.phi10
  %index.next = add nuw i32 %index, 32
  %vec.ind.next = add <8 x i32> %vec.ind, <i32 -32, i32 -32, i32 -32, i32 -32, i32 -32, i32 -32, i32 -32, i32 -32>
  %9 = icmp eq i32 %index.next, %n.vec
  br i1 %9, label %middle.block, label %vector.body, !llvm.loop !0

middle.block:                                     ; preds = %vector.body
  %bin.rdx = mul <8 x i32> %6, %5
  %bin.rdx11 = mul <8 x i32> %7, %bin.rdx
  %bin.rdx12 = mul <8 x i32> %8, %bin.rdx11
  %10 = tail call i32 @llvm.vector.reduce.mul.v8i32(<8 x i32> %bin.rdx12)
  %cmp.n = icmp eq i32 %4, %n.vec
  br i1 %cmp.n, label %common.ret, label %if.exit.L2.preheader13

if.exit.L2.preheader13:                           ; preds = %if.exit.L2.preheader, %middle.block
  %.tr4.ph = phi i32 [ %0, %if.exit.L2.preheader ], [ %ind.end, %middle.block ]
  %accumulator.tr3.ph = phi i32 [ 1, %if.exit.L2.preheader ], [ %10, %middle.block ]
  br label %if.exit.L2

common.ret:                                       ; preds = %if.exit.L2, %middle.block, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %10, %middle.block ], [ %12, %if.exit.L2 ]
  ret i32 %accumulator.tr.lcssa

if.exit.L2:                                       ; preds = %if.exit.L2.preheader13, %if.exit.L2
  %.tr4 = phi i32 [ %11, %if.exit.L2 ], [ %.tr4.ph, %if.exit.L2.preheader13 ]
  %accumulator.tr3 = phi i32 [ %12, %if.exit.L2 ], [ %accumulator.tr3.ph, %if.exit.L2.preheader13 ]
  %11 = add nsw i32 %.tr4, -1
  %12 = mul i32 %.tr4, %accumulator.tr3
  %13 = icmp ult i32 %.tr4, 3
  br i1 %13, label %common.ret, label %if.exit.L2, !llvm.loop !3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc i32 @_f__void__int__faculty__int(i32 10) #5
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 10, i32 %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.vector.reduce.mul.v8i32(<8 x i32>) #3

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.usub.sat.i32(i32, i32) #4

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #4 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #5 = { nounwind }

!0 = distinct !{!0, !1, !2}
!1 = !{!"llvm.loop.isvectorized", i32 1}
!2 = !{!"llvm.loop.unroll.runtime.disable"}
!3 = distinct !{!3, !2, !1}
