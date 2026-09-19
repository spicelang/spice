; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.TestStruct = type { i64, %struct.String, i32 }
%struct.String = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"test string\00", align 4
@printf.str.0 = private unnamed_addr constant [10 x i8] c"Long: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [9 x i8] c"Int: %d\0A\00", align 4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN10TestStruct4dtorEv(ptr noundef nonnull align 8 dereferenceable(40) %0) #0 !dbg !9 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8, !dbg !13
  %2 = load ptr, ptr %this, align 8, !dbg !13
  %3 = getelementptr inbounds nuw %struct.TestStruct, ptr %2, i32 0, i32 1, !dbg !13
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %3), !dbg !13
  ret void, !dbg !13
}

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef %struct.TestStruct @_Z3fctRi(ptr noundef %0) #1 !dbg !14 {
  %ref = alloca ptr, align 8
  %2 = alloca %struct.String, align 8
  %ts = alloca %struct.TestStruct, align 8
  store ptr %0, ptr %ref, align 8, !dbg !15
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %2, ptr noundef @anon.string.0), !dbg !16
  store i64 6, ptr %ts, align 8, !dbg !17
  %3 = load %struct.String, ptr %2, align 8, !dbg !17
  %4 = getelementptr inbounds nuw %struct.TestStruct, ptr %ts, i32 0, i32 1, !dbg !17
  store %struct.String %3, ptr %4, align 8, !dbg !17
  %5 = load ptr, ptr %ref, align 8, !dbg !17
  %6 = load i32, ptr %5, align 4, !dbg !17
  %7 = getelementptr inbounds nuw %struct.TestStruct, ptr %ts, i32 0, i32 2, !dbg !17
  store i32 %6, ptr %7, align 4, !dbg !17
  %8 = load %struct.TestStruct, ptr %ts, align 8, !dbg !18
  ret %struct.TestStruct %8, !dbg !19
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 !dbg !20 {
  %test = alloca i32, align 4
  %res = alloca %struct.TestStruct, align 8
  store i32 987654, ptr %test, align 4, !dbg !21
  %1 = call noundef %struct.TestStruct @_Z3fctRi(ptr noundef %test), !dbg !22
  store %struct.TestStruct %1, ptr %res, align 8, !dbg !22
  %lng.addr = getelementptr inbounds %struct.TestStruct, ptr %res, i64 0, i32 0, !dbg !23
  %2 = load i64, ptr %lng.addr, align 8, !dbg !23
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 noundef %2), !dbg !23
  %4 = getelementptr inbounds nuw %struct.TestStruct, ptr %res, i32 0, i32 1, !dbg !24
  %5 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %4), !dbg !24
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr noundef %5), !dbg !24
  %i.addr = getelementptr inbounds %struct.TestStruct, ptr %res, i64 0, i32 2, !dbg !25
  %7 = load i32, ptr %i.addr, align 4, !dbg !25
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %7), !dbg !25
  call void @_ZN10TestStruct4dtorEv(ptr noundef nonnull align 8 dereferenceable(40) %res), !dbg !26
  ret i32 0, !dbg !26
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

declare ptr @_ZN6String6getRawEv(ptr)

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!6}
!llvm.dbg.cu = !{!7}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!7 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !8, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly, splitDebugInlining: false, nameTableKind: None)
!8 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/instrumentation/success-dbg-info-line-only/source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-line-only")
!9 = distinct !DISubprogram(name: "dtor", linkageName: "_ZN10TestStruct4dtorEv", scope: !10, file: !10, line: 3, type: !11, scopeLine: 3, flags: DIFlagPublic | DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!10 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-line-only")
!11 = !DISubroutineType(types: !12)
!12 = !{}
!13 = !DILocation(line: 3, column: 1, scope: !9)
!14 = distinct !DISubprogram(name: "fct", linkageName: "_Z3fctRi", scope: !10, file: !10, line: 9, type: !11, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!15 = !DILocation(line: 9, column: 19, scope: !14)
!16 = !DILocation(line: 10, column: 44, scope: !14)
!17 = !DILocation(line: 10, column: 60, scope: !14)
!18 = !DILocation(line: 11, column: 12, scope: !14)
!19 = !DILocation(line: 12, column: 1, scope: !14)
!20 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !10, file: !10, line: 14, type: !11, scopeLine: 14, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!21 = !DILocation(line: 15, column: 16, scope: !20)
!22 = !DILocation(line: 16, column: 32, scope: !20)
!23 = !DILocation(line: 17, column: 26, scope: !20)
!24 = !DILocation(line: 18, column: 28, scope: !20)
!25 = !DILocation(line: 19, column: 25, scope: !20)
!26 = !DILocation(line: 20, column: 1, scope: !20)
