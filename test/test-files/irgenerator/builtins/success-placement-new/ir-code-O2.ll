; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 4

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write)
define private fastcc void @_ZN5Point4ctorEii(ptr noundef nonnull writeonly align 4 captures(none) dereferenceable(8) initializes((0, 8)) %0, i32 noundef range(i32 0, 8) %1, i32 noundef range(i32 0, 12) %2) unnamed_addr #0 {
  %4 = getelementptr inbounds nuw i8, ptr %0, i64 4
  store i32 %1, ptr %0, align 4
  store i32 %2, ptr %4, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 {
  %storage = alloca ptr, align 8
  %1 = tail call ptr @_Z12sAllocUnsafem(i64 8) #3
  tail call fastcc void @_ZN5Point4ctorEii(ptr noundef nonnull align 4 dereferenceable(8) %1, i32 0, i32 0) #3
  store ptr %1, ptr %storage, align 8
  tail call fastcc void @_ZN5Point4ctorEii(ptr noundef nonnull align 4 dereferenceable(8) %1, i32 7, i32 11) #3
  %2 = load i32, ptr %1, align 4
  %y.addr = getelementptr inbounds nuw i8, ptr %1, i64 4
  %3 = load i32, ptr %y.addr, align 4
  %4 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef %2, i32 noundef %3)
  call void @_Z8sDeallocRPh(ptr nonnull %storage) #3
  ret i32 0
}

declare ptr @_Z12sAllocUnsafem(i64) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

declare void @_Z8sDeallocRPh(ptr) local_unnamed_addr

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
