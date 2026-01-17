; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Faculty of %d is: %d\00", align 4

; Function Attrs: nofree norecurse nosync nounwind memory(none)
define private fastcc noundef i32 @_Z7facultyi(i32 noundef range(i32 1, 11) %0) unnamed_addr #0 {
  %2 = icmp samesign ult i32 %0, 2
  br i1 %2, label %common.ret, label %if.exit.L2.preheader

if.exit.L2.preheader:                             ; preds = %1
  %3 = add nsw i32 %0, -1
  %4 = add nsw i32 %0, -2
  %xtraiter = and i32 %3, 7
  %5 = icmp ult i32 %4, 7
  br i1 %5, label %if.exit.L2.epil.preheader, label %if.exit.L2.preheader.new

if.exit.L2.preheader.new:                         ; preds = %if.exit.L2.preheader
  %unroll_iter = and i32 %3, -8
  br label %if.exit.L2

common.ret.loopexit.unr-lcssa:                    ; preds = %if.exit.L2
  %lcmp.mod.not = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod.not, label %common.ret, label %if.exit.L2.epil.preheader

if.exit.L2.epil.preheader:                        ; preds = %common.ret.loopexit.unr-lcssa, %if.exit.L2.preheader
  %.tr4.epil.init = phi i32 [ %0, %if.exit.L2.preheader ], [ %22, %common.ret.loopexit.unr-lcssa ]
  %accumulator.tr3.epil.init = phi i32 [ 1, %if.exit.L2.preheader ], [ %23, %common.ret.loopexit.unr-lcssa ]
  %lcmp.mod6 = icmp ne i32 %xtraiter, 0
  tail call void @llvm.assume(i1 %lcmp.mod6)
  br label %if.exit.L2.epil

if.exit.L2.epil:                                  ; preds = %if.exit.L2.epil, %if.exit.L2.epil.preheader
  %.tr4.epil = phi i32 [ %6, %if.exit.L2.epil ], [ %.tr4.epil.init, %if.exit.L2.epil.preheader ]
  %accumulator.tr3.epil = phi i32 [ %7, %if.exit.L2.epil ], [ %accumulator.tr3.epil.init, %if.exit.L2.epil.preheader ]
  %epil.iter = phi i32 [ %epil.iter.next, %if.exit.L2.epil ], [ 0, %if.exit.L2.epil.preheader ]
  %6 = add nsw i32 %.tr4.epil, -1
  %7 = mul nsw i32 %.tr4.epil, %accumulator.tr3.epil
  %epil.iter.next = add i32 %epil.iter, 1
  %epil.iter.cmp.not = icmp eq i32 %epil.iter.next, %xtraiter
  br i1 %epil.iter.cmp.not, label %common.ret, label %if.exit.L2.epil, !llvm.loop !5

common.ret:                                       ; preds = %common.ret.loopexit.unr-lcssa, %if.exit.L2.epil, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %23, %common.ret.loopexit.unr-lcssa ], [ %7, %if.exit.L2.epil ]
  ret i32 %accumulator.tr.lcssa

if.exit.L2:                                       ; preds = %if.exit.L2, %if.exit.L2.preheader.new
  %.tr4 = phi i32 [ %0, %if.exit.L2.preheader.new ], [ %22, %if.exit.L2 ]
  %accumulator.tr3 = phi i32 [ 1, %if.exit.L2.preheader.new ], [ %23, %if.exit.L2 ]
  %niter = phi i32 [ 0, %if.exit.L2.preheader.new ], [ %niter.next.7, %if.exit.L2 ]
  %8 = add nsw i32 %.tr4, -1
  %9 = mul nsw i32 %.tr4, %accumulator.tr3
  %10 = add nsw i32 %.tr4, -2
  %11 = mul nsw i32 %8, %9
  %12 = add nsw i32 %.tr4, -3
  %13 = mul nsw i32 %10, %11
  %14 = add nsw i32 %.tr4, -4
  %15 = mul nsw i32 %12, %13
  %16 = add nsw i32 %.tr4, -5
  %17 = mul nsw i32 %14, %15
  %18 = add nsw i32 %.tr4, -6
  %19 = mul nsw i32 %16, %17
  %20 = add nsw i32 %.tr4, -7
  %21 = mul nsw i32 %18, %19
  %22 = add nsw i32 %.tr4, -8
  %23 = mul nsw i32 %20, %21
  %niter.next.7 = add i32 %niter, 8
  %niter.ncmp.7 = icmp eq i32 %niter.next.7, %unroll_iter
  br i1 %niter.ncmp.7, label %common.ret.loopexit.unr-lcssa, label %if.exit.L2
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 {
  %1 = tail call fastcc noundef i32 @_Z7facultyi(i32 noundef 10) #4
  %2 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef 10, i32 noundef %1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write)
declare void @llvm.assume(i1 noundef) #3

attributes #0 = { nofree norecurse nosync nounwind memory(none) }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(inaccessiblemem: write) }
attributes #4 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.unroll.disable"}
