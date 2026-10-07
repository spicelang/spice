; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Outer = type { ptr, i64, %struct.Inner }
%struct.Inner = type { ptr, i32 }

@printf.str.0 = private unnamed_addr constant [30 x i8] c"Root inner parent is nil: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [38 x i8] c"Child inner parent is root inner: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [23 x i8] c"Root parent value: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [24 x i8] c"Child parent value: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [18 x i8] c"Depth of nil: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [19 x i8] c"Depth of root: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [20 x i8] c"Depth of child: %d\0A\00", align 4

; Function Attrs: nofree noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  %root = alloca %struct.Outer, align 8
  %child.sroa.3 = alloca ptr, align 8
  %inner.addr.i = getelementptr inbounds nuw i8, ptr %root, i64 16
  %.sroa.2.0.inner.addr.sroa_idx.i = getelementptr inbounds nuw i8, ptr %root, i64 24
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 8 dereferenceable(24) %root, i8 0, i64 24, i1 false)
  store i32 1, ptr %.sroa.2.0.inner.addr.sroa_idx.i, align 8
  store ptr %inner.addr.i, ptr %child.sroa.3, align 8
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef 1)
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 noundef 1)
  %3 = load ptr, ptr %root, align 8
  %4 = icmp eq ptr %3, null
  br i1 %4, label %_ZN5Outer14getParentValueEP5Outer.exit13, label %if.exit.L25.i

if.exit.L25.i:                                    ; preds = %0
  %value.addr.i = getelementptr inbounds nuw i8, ptr %3, i64 24
  %5 = load i32, ptr %value.addr.i, align 4
  br label %_ZN5Outer14getParentValueEP5Outer.exit13

_ZN5Outer14getParentValueEP5Outer.exit13:         ; preds = %0, %if.exit.L25.i
  %common.ret.op.i = phi i32 [ %5, %if.exit.L25.i ], [ -1, %0 ]
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i32 noundef %common.ret.op.i)
  %7 = load i32, ptr %.sroa.2.0.inner.addr.sroa_idx.i, align 8
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 noundef %7)
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4, i32 noundef 0)
  br label %while.body.L34.i

while.body.L34.i:                                 ; preds = %_ZN5Outer14getParentValueEP5Outer.exit13, %while.body.L34.i
  %depth.05.i = phi i32 [ %10, %while.body.L34.i ], [ 0, %_ZN5Outer14getParentValueEP5Outer.exit13 ]
  %inner.04.i = phi ptr [ %11, %while.body.L34.i ], [ %inner.addr.i, %_ZN5Outer14getParentValueEP5Outer.exit13 ]
  %10 = add nuw nsw i32 %depth.05.i, 1
  %11 = load ptr, ptr %inner.04.i, align 8
  %.not.i = icmp eq ptr %11, null
  br i1 %.not.i, label %_Z8getDepthPK5Inner.exit, label %while.body.L34.i

_Z8getDepthPK5Inner.exit:                         ; preds = %while.body.L34.i
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i32 noundef %10)
  br label %while.body.L34.i14

while.body.L34.i14:                               ; preds = %_Z8getDepthPK5Inner.exit, %while.body.L34.i14
  %depth.05.i15 = phi i32 [ %13, %while.body.L34.i14 ], [ 0, %_Z8getDepthPK5Inner.exit ]
  %inner.04.i16 = phi ptr [ %14, %while.body.L34.i14 ], [ %child.sroa.3, %_Z8getDepthPK5Inner.exit ]
  %13 = add nuw nsw i32 %depth.05.i15, 1
  %14 = load ptr, ptr %inner.04.i16, align 8
  %.not.i17 = icmp eq ptr %14, null
  br i1 %.not.i17, label %_Z8getDepthPK5Inner.exit18, label %while.body.L34.i14

_Z8getDepthPK5Inner.exit18:                       ; preds = %while.body.L34.i14
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.6, i32 noundef %13)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #2

attributes #0 = { nofree noinline norecurse nounwind uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
