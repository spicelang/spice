; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Thread = type { %struct.Lambda, i64 }
%struct.Lambda = type { { ptr, ptr, i64 }, ptr, i64 }

@COUNTER = private global i32 0, !dbg !0
@llvm.used = appending global [1 x ptr] [ptr @tsan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @tsan.module_ctor, ptr null }]

; Function Attrs: sanitize_thread
define private void @_Z6workerv() #0 !dbg !15 {
  %1 = call ptr @llvm.returnaddress.p0(i32 0), !dbg !20
  call void @__tsan_func_entry(ptr %1), !dbg !20
  %i = alloca i32, align 4
    #dbg_declare(ptr %i, !21, !DIExpression(), !23)
  store i32 0, ptr %i, align 4, !dbg !23
  br label %for.head.L8, !dbg !23

for.head.L8:                                      ; preds = %for.tail.L8, %0
  %2 = load i32, ptr %i, align 4, !dbg !24
  %3 = icmp slt i32 %2, 1000000, !dbg !24
  br i1 %3, label %for.body.L8, label %for.exit.L8, !dbg !24

for.body.L8:                                      ; preds = %for.head.L8
  %4 = load i32, ptr @COUNTER, align 4, !dbg !25
  %5 = add nsw i32 %4, 1, !dbg !25
  call void @__tsan_write4(ptr @COUNTER), !dbg !25
  store i32 %5, ptr @COUNTER, align 4, !dbg !25
  br label %for.tail.L8, !dbg !26

for.tail.L8:                                      ; preds = %for.body.L8
  %6 = load i32, ptr %i, align 4, !dbg !27
  %7 = add nsw i32 %6, 1, !dbg !27
  store i32 %7, ptr %i, align 4, !dbg !27
  br label %for.head.L8, !dbg !27

for.exit.L8:                                      ; preds = %for.head.L8
  call void @__tsan_func_exit(), !dbg !28
  ret void, !dbg !28
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable
define dso_local noundef i32 @main() #1 !dbg !29 {
  %1 = call ptr @llvm.returnaddress.p0(i32 0), !dbg !32
  call void @__tsan_func_entry(ptr %1), !dbg !32
  %result = alloca i32, align 4
  %thread1 = alloca %struct.Thread, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %thread2 = alloca %struct.Thread, align 8
  %fat.ptr1 = alloca { ptr, ptr, i64 }, align 8
    #dbg_declare(ptr %result, !33, !DIExpression(), !34)
  store i32 0, ptr %result, align 4, !dbg !34
  store ptr @_Z6workerv.fatthunk, ptr %fat.ptr, align 8, !dbg !35
  %2 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1, !dbg !35
  store ptr null, ptr %2, align 8, !dbg !35
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2, !dbg !35
  store i64 0, ptr %3, align 8, !dbg !35
  %4 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8, !dbg !35
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %thread1, { ptr, ptr, i64 } noundef %4), !dbg !35
    #dbg_declare(ptr %thread1, !36, !DIExpression(), !35)
  store ptr @_Z6workerv.fatthunk, ptr %fat.ptr1, align 8, !dbg !56
  %5 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 1, !dbg !56
  store ptr null, ptr %5, align 8, !dbg !56
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 2, !dbg !56
  store i64 0, ptr %6, align 8, !dbg !56
  %7 = load { ptr, ptr, i64 }, ptr %fat.ptr1, align 8, !dbg !56
  call void @_ZN6Thread4ctorEPFvE(ptr noundef nonnull align 8 dereferenceable(48) %thread2, { ptr, ptr, i64 } noundef %7), !dbg !56
    #dbg_declare(ptr %thread2, !57, !DIExpression(), !56)
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !58
  call void @_ZN6Thread3runEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !59
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !60
  call void @_ZN6Thread4joinEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !61
  call void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %thread2), !dbg !62
  call void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48) %thread1), !dbg !62
  %8 = load i32, ptr %result, align 4, !dbg !62
  call void @__tsan_func_exit(), !dbg !62
  ret i32 %8, !dbg !62
}

define private void @_Z6workerv.fatthunk(ptr %0) personality ptr @__gcc_personality_v0 {
entry:
  %1 = call ptr @llvm.returnaddress.p0(i32 0), !dbg !35
  call void @__tsan_func_entry(ptr %1), !dbg !35
  invoke void @_Z6workerv()
          to label %.noexc unwind label %tsan_cleanup, !dbg !35

.noexc:                                           ; preds = %entry
  call void @__tsan_func_exit(), !dbg !35
  ret void, !dbg !35

tsan_cleanup:                                     ; preds = %entry
  %cleanup.lpad = landingpad { ptr, i32 }
          cleanup
  call void @__tsan_func_exit()
  resume { ptr, i32 } %cleanup.lpad
}

declare void @_ZN6Thread4ctorEPFvE(ptr, { ptr, ptr, i64 })

declare void @_ZN6Thread3runEv(ptr)

declare void @_ZN6Thread4joinEv(ptr)

declare void @_ZN6Thread4dtorEv(ptr noundef nonnull align 8 dereferenceable(48))

declare void @__tsan_init()

; Function Attrs: nounwind uwtable
define internal void @tsan.module_ctor() #2 {
  call void @__tsan_init()
  ret void
}

; Function Attrs: nounwind
declare void @__tsan_func_entry(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_func_exit() #3

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_begin() #3

; Function Attrs: nounwind
declare void @__tsan_ignore_thread_end() #3

; Function Attrs: nounwind
declare void @__tsan_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write1(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write1(ptr) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic8_store(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_exchange(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_add(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_sub(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_and(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_nand(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_or(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_fetch_xor(ptr, i8, i32) #3

; Function Attrs: nounwind
declare i8 @__tsan_atomic8_compare_exchange_val(ptr, i8, i8, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write2(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write2(ptr) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic16_store(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_exchange(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_add(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_sub(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_and(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_nand(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_or(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_fetch_xor(ptr, i16, i32) #3

; Function Attrs: nounwind
declare i16 @__tsan_atomic16_compare_exchange_val(ptr, i16, i16, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write4(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write4(ptr) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic32_store(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_exchange(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_add(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_sub(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_and(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_nand(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_or(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_fetch_xor(ptr, i32, i32) #3

; Function Attrs: nounwind
declare i32 @__tsan_atomic32_compare_exchange_val(ptr, i32, i32, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write8(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write8(ptr) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic64_store(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_exchange(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_add(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_sub(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_and(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_nand(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_or(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_fetch_xor(ptr, i64, i32) #3

; Function Attrs: nounwind
declare i64 @__tsan_atomic64_compare_exchange_val(ptr, i64, i64, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_volatile_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_read16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_volatile_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_read_write16(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_unaligned_read_write16(ptr) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_load(ptr, i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic128_store(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_exchange(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_add(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_sub(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_and(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_nand(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_or(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_fetch_xor(ptr, i128, i32) #3

; Function Attrs: nounwind
declare i128 @__tsan_atomic128_compare_exchange_val(ptr, i128, i128, i32, i32) #3

; Function Attrs: nounwind
declare void @__tsan_vptr_update(ptr, ptr) #3

; Function Attrs: nounwind
declare void @__tsan_vptr_read(ptr) #3

; Function Attrs: nounwind
declare void @__tsan_atomic_thread_fence(i32) #3

; Function Attrs: nounwind
declare void @__tsan_atomic_signal_fence(i32) #3

; Function Attrs: nounwind
declare ptr @__tsan_memmove(ptr, ptr, i64) #3

; Function Attrs: nounwind
declare ptr @__tsan_memcpy(ptr, ptr, i64) #3

; Function Attrs: nounwind
declare ptr @__tsan_memset(ptr, i32, i64) #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare ptr @llvm.returnaddress.p0(i32 immarg) #4

declare i32 @__gcc_personality_v0(...)

attributes #0 = { sanitize_thread }
attributes #1 = { mustprogress noinline norecurse nounwind optnone sanitize_thread uwtable }
attributes #2 = { nounwind uwtable "frame-pointer"="all" }
attributes #3 = { nounwind }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(none) }

!llvm.module.flags = !{!7, !8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "COUNTER", linkageName: "COUNTER", scope: !2, file: !5, line: 5, type: !6, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/instrumentation/success-tsan-and-dbg-info/source.spice", directory: "./test-files/irgenerator/instrumentation/success-tsan-and-dbg-info")
!4 = !{!0}
!5 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-tsan-and-dbg-info")
!6 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!7 = !{i32 8, !"PIC Level", i32 2}
!8 = !{i32 7, !"PIE Level", i32 2}
!9 = !{i32 7, !"uwtable", i32 2}
!10 = !{i32 7, !"frame-pointer", i32 2}
!11 = !{i32 7, !"Dwarf Version", i32 5}
!12 = !{i32 2, !"Debug Info Version", i32 3}
!13 = !{i32 4, !"nosanitize_thread", i32 1}
!14 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!15 = distinct !DISubprogram(name: "worker", linkageName: "_Z6workerv", scope: !5, file: !5, line: 7, type: !16, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !19)
!16 = !DISubroutineType(types: !17)
!17 = !{!18}
!18 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!19 = !{}
!20 = !DILocation(line: 0, scope: !15)
!21 = !DILocalVariable(name: "i", scope: !22, file: !5, line: 8, type: !6)
!22 = distinct !DILexicalBlock(scope: !15, file: !5, line: 8, column: 5)
!23 = !DILocation(line: 8, column: 17, scope: !22)
!24 = !DILocation(line: 8, column: 24, scope: !22)
!25 = !DILocation(line: 9, column: 9, scope: !22)
!26 = !DILocation(line: 10, column: 5, scope: !22)
!27 = !DILocation(line: 8, column: 33, scope: !22)
!28 = !DILocation(line: 11, column: 1, scope: !15)
!29 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !5, file: !5, line: 13, type: !30, scopeLine: 13, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !19)
!30 = !DISubroutineType(types: !31)
!31 = !{!6}
!32 = !DILocation(line: 0, scope: !29)
!33 = !DILocalVariable(name: "result", scope: !29, file: !5, line: 13, type: !6)
!34 = !DILocation(line: 13, column: 1, scope: !29)
!35 = !DILocation(line: 14, column: 29, scope: !29)
!36 = !DILocalVariable(name: "thread1", scope: !29, file: !5, line: 14, type: !37)
!37 = !DICompositeType(tag: DW_TAG_structure_type, name: "Thread", scope: !5, file: !5, line: 23, size: 384, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !38, identifier: "struct.Thread")
!38 = !{!39, !54}
!39 = !DIDerivedType(tag: DW_TAG_member, name: "threadRoutine", scope: !37, file: !5, line: 24, baseType: !40, size: 320, align: 8)
!40 = !DICompositeType(tag: DW_TAG_structure_type, name: "Lambda", scope: !5, file: !5, line: 31, size: 320, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !41, identifier: "struct.Lambda")
!41 = !{!42, !50, !53}
!42 = !DIDerivedType(tag: DW_TAG_member, name: "native", scope: !40, file: !5, line: 32, baseType: !43, size: 192, align: 8)
!43 = !DICompositeType(tag: DW_TAG_structure_type, name: "_lambda", scope: !5, file: !5, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !44, identifier: "_lambda")
!44 = !{!45, !47, !48}
!45 = !DIDerivedType(tag: DW_TAG_member, name: "fct", scope: !43, file: !5, baseType: !46, size: 64, align: 8)
!46 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64, align: 8)
!47 = !DIDerivedType(tag: DW_TAG_member, name: "captures", scope: !43, file: !5, baseType: !46, size: 64, align: 8, offset: 64)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !43, file: !5, baseType: !49, size: 64, align: 8, offset: 128)
!49 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!50 = !DIDerivedType(tag: DW_TAG_member, name: "ownedCaptures", scope: !40, file: !5, line: 33, baseType: !51, size: 64, offset: 192)
!51 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !52, size: 64)
!52 = !DIBasicType(name: "byte", size: 8, encoding: DW_ATE_unsigned)
!53 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !40, file: !5, line: 34, baseType: !49, size: 64, offset: 256)
!54 = !DIDerivedType(tag: DW_TAG_member, name: "threadId", scope: !37, file: !5, line: 25, baseType: !55, size: 64, offset: 320)
!55 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!56 = !DILocation(line: 15, column: 29, scope: !29)
!57 = !DILocalVariable(name: "thread2", scope: !29, file: !5, line: 15, type: !37)
!58 = !DILocation(line: 16, column: 5, scope: !29)
!59 = !DILocation(line: 17, column: 5, scope: !29)
!60 = !DILocation(line: 18, column: 5, scope: !29)
!61 = !DILocation(line: 19, column: 5, scope: !29)
!62 = !DILocation(line: 20, column: 1, scope: !29)
