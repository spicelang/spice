; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { %struct.Outer }
%struct.Outer = type { i32 }

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %_ts = alloca %struct.TestStruct, align 8
  call void @_ZN10TestStructI5OuterE4ctorEv(ptr noundef nonnull align 4 dereferenceable(4) %_ts)
  ret i32 0
}

declare void @_ZN10TestStructI5OuterE4ctorEv(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
